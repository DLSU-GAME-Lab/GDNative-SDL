#version 450
// ---------------------------------------------------------------------------
// sprite_shimmer.frag : per-sprite shimmer with optional normal map and mask.
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
    float mask = texture(u_mask, v_uv).r;

    vec3 glint = p.lightColor.rgb * (spec + 0.25 * diff) * att * p.a.y * mask;

    // Alpha is untouched, so the glint stays inside the sprite's silhouette.
    o_color = vec4(base.rgb + glint, base.a);
}
