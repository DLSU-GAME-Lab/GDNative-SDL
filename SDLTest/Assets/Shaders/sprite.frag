// assets/shaders/sprite.frag
#version 450

// --- input from vertex shader ---
layout(location = 0) in vec2 fragUV;

// --- output ---
layout(location = 0) out vec4 outColor;

// --- textures ---
layout(set = 2, binding = 0) uniform sampler2D uDiffuse;    // your regular sprite texture
layout(set = 2, binding = 1) uniform sampler2D uNormalMap;  // normal map

// --- uniform buffer: lighting + effect data pushed per draw call ---
layout(set = 3, binding = 0) uniform FragUniforms {
    vec4  color;        // tint color (r,g,b,a) — matches your SDL_Color
    float time;         // elapsed time in seconds
    float phase;        // shimmer sweep position (0.0 - 1.0)
    float shimmerWidth; // how wide the shimmer band is
    float shimmerPower; // how intense the shimmer is
} uFrag;

// hardcoded light direction for now — replace with a uniform later when you add a light source
// xyz = direction, points toward the viewer slightly (z = 1.0 means straight on)
const vec3 LIGHT_DIR = normalize(vec3(0.0, 0.0, 1.0));

void main()
{
    // --- 1. sample diffuse texture ---
    vec4 diffuse = texture(uDiffuse, fragUV);

    // discard fully transparent pixels so normal map doesn't bleed outside sprite edges
    if (diffuse.a < 0.01) discard;

    // --- 2. sample normal map ---
    // normal maps store directions as rgb (0-1), remap to (-1 to 1)
    vec3 normal = texture(uNormalMap, fragUV).rgb;
    normal = normalize(normal * 2.0 - 1.0);

    // --- 3. basic diffuse lighting using the normal ---
    // dot product of normal vs light direction
    // higher value = surface faces the light = brighter
    float diffuseLight = max(dot(normal, LIGHT_DIR), 0.0);

    // ambient so the unlit side isn't completely black
    float ambient = 0.4;
    float lighting = ambient + (1.0 - ambient) * diffuseLight;

    // --- 4. shimmer using normal map ---
    // instead of a flat horizontal stripe, use the normal's x component
    // so the shimmer follows the surface contour of the sprite
    float normalInfluence = normal.x * 0.5 + 0.5; // remap -1..1 to 0..1

    // sweep position blended with surface normal
    float sweepPos   = fragUV.x + normalInfluence * 0.2;
    float dist       = abs(sweepPos - uFrag.phase);
    float shimmer    = exp(-(dist * dist) / (uFrag.shimmerWidth * uFrag.shimmerWidth));
    shimmer         *= uFrag.shimmerPower;

    // only shimmer on surfaces facing somewhat toward the viewer
    shimmer *= smoothstep(0.0, 0.5, normal.z);

    // --- 5. combine everything ---
    vec3 litColor  = diffuse.rgb * uFrag.color.rgb * lighting;
    vec3 finalColor = litColor + vec3(shimmer);

    outColor = vec4(finalColor, diffuse.a * uFrag.color.a);
}