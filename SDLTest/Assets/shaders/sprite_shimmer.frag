#version 450
// ---------------------------------------------------------------------------
// sprite_shimmer.frag : per-sprite lighting + shimmer with optional normal map and mask.
// Fragment-stage layout follows SDL_GPU's SPIR-V convention:
//   set 2 = sampled textures (binding 0 = the sprite SDL is drawing,
//                             binding 1 = the normal map we attach,
//                             binding 2 = the shimmer mask we attach, red channel)
//   set 3 = fragment uniform buffers (slot 0)
// Inputs come from SDL's built-in GPU-renderer vertex shader.
// ---------------------------------------------------------------------------
layout(location = 0) in vec4 v_color;   // texture color/alpha mod
layout(location = 1) in vec2 v_uv;      // uv inside the sprite's texture
layout(location = 0) out vec4 o_color;

layout(set = 2, binding = 0) uniform sampler2D u_texture;
layout(set = 2, binding = 1) uniform sampler2D u_normal;
layout(set = 2, binding = 2) uniform sampler2D u_mask;

layout(set = 3, binding = 0, std140) uniform ShimmerParams {
    vec4 a;          // x = phase (0..1), y = intensity, z = shininess, w = band width (uv units)
    vec4 lightColor; // rgb = glint color, w = normal strength
    vec4 b;          // x = flipX (+1/-1), y = flipY (+1/-1), z = light height, w = green sign (+1 OpenGL, -1 DirectX normal maps)
    vec4 c;          // x = lit amount, y = ambient, z = highlight strength, w = highlight exponent (static lighting)
    vec4 d;          // xyz = key light direction, w = share of the sweep shown outside the mask
    vec4 e;          // x = desaturation of the lit colour, y = how much brighter sides lighten towards white
} p;

void main()
{
    vec4 base = texture(u_texture, v_uv) * v_color;

    // Tangent-space normal, corrected for sprite flips and normal-map convention.
    vec3 n = texture(u_normal, v_uv).xyz * 2.0 - 1.0;
    n.xy *= p.lightColor.w;
    n.x  *= p.b.x;
    n.y  *= p.b.y * p.b.w;
    n.z   = max(n.z, 0.05);
    n     = normalize(n);

    // Static key light, so the flat art reads as a lit 3D object. Normalised so a flat
    // surface keeps the art's own colour; surfaces facing the light get brighter.
    vec3  Ls      = normalize(p.d.xyz);
    float ambient = p.c.y;
    float shade   = (ambient + (1.0 - ambient) * max(dot(n, Ls), 0.0))
                  / (ambient + (1.0 - ambient) * Ls.z);
    shade = mix(1.0, shade, p.c.x);

    vec3  Hs   = normalize(Ls + vec3(0.0, 0.0, 1.0));
    float spot = pow(max(dot(n, Hs), 0.0), p.c.w) * p.c.z * p.c.x;

    // Darker sides darken the art, brighter sides lighten towards white rather than
    // piling up saturation, then the result is pulled slightly towards grey.
    vec3 lit = base.rgb * min(shade, 1.0)
             + max(shade - 1.0, 0.0) * mix(base.rgb, 1.0 - base.rgb, p.e.y)
             + vec3(spot);
    lit = mix(lit, vec3(dot(lit, vec3(0.2126, 0.7152, 0.0722))), p.e.x);

    // Screen-oriented sprite coordinates (0..1, left->right, top->bottom).
    vec2 s = vec2(p.b.x < 0.0 ? 1.0 - v_uv.x : v_uv.x,
                  p.b.y < 0.0 ? 1.0 - v_uv.y : v_uv.y);

    // A point light sweeping diagonally across the sprite, in and out of frame.
    float phase = p.a.x;
    vec3 lightPos = vec3(mix(-0.35, 1.35, phase), mix(0.15, 0.75, phase), p.b.z);

    vec3 L = normalize(lightPos - vec3(s, 0.0));
    vec3 H = normalize(L + vec3(0.0, 0.0, 1.0));

    float spec = pow(max(dot(n, H), 0.0), p.a.z);
    float diff = max(dot(n, L), 0.0);

    // Gaussian falloff around the light so the glint reads as a band.
    vec2  d   = vec2(s.x - lightPos.x, (s.y - lightPos.y) * 0.5);
    float att = exp(-dot(d, d) / (p.a.w * p.a.w));

    // The mask (red channel) limits the glint to the parts of the sprite that
    // should catch the light. A white mask means the whole sprite shimmers.
    // The sweep is off while the phase is negative.
    float mask  = mix(p.d.w, 1.0, texture(u_mask, v_uv).r);
    float sweep = phase >= 0.0 ? 1.0 : 0.0;

    vec3 glint = p.lightColor.rgb * (spec + 0.25 * diff) * att * p.a.y * mask * sweep;

    // Alpha is untouched, so the glint stays inside the sprite's silhouette.
    o_color = vec4(lit + glint, base.a);
}
