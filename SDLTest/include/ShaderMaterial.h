#pragma once
#include <SDL3/SDL.h>

// Must match `ShimmerParams` in shaders/sprite_shimmer.frag (std140, 96 bytes).
struct ShimmerUniforms
{
    // vec4 a
    float phase = 0.0f;   // 0..1 sweep position, -1 = no sweep (static lighting only)
    float intensity = 1.5f;   // glint brightness
    float shininess = 8.0f;   // specular exponent (higher = tighter highlight)
    float bandWidth = 0.40f;  // gaussian width of the sweep, in uv units
    // vec4 lightColor
    float lightR = 1.0f;
    float lightG = 1.0f;
    float lightB = 1.0f;
    float normalStrength = 1.0f; // scales normal-map xy (0 = flat)
    // vec4 b
    float flipX = 1.0f;   // +1 / -1
    float flipY = 1.0f;   // +1 / -1
    float lightHeight = 0.35f;  // light z above the sprite (lower = more dramatic)
    float greenSign = 1.0f;   // +1 OpenGL-style normal maps, -1 DirectX-style
    // vec4 c
    float litAmount = 1.0f;     // static lighting from the normal map, 0 = off, 1 = full
    float litAmbient = 0.85f;   // light level of surfaces facing away from the key light
    float specStrength = 0.35f;  // brightness of the static highlight
    float specPower = 14.0f;    // static highlight exponent (higher = tighter highlight)
    // vec4 d
    float keyX = -0.45f;        // key light direction (x right, y up, z towards the viewer)
    float keyY = 0.55f;
    float keyZ = 0.7f;
    float bodyGlint = 0.35f;    // share of the sweep that shows outside the mask
    // vec4 e
    float litDesaturate = 0.08f; // pulls the lit colour towards grey, 0 = keep the art's saturation
    float litWhiten = 0.6f;     // how much brighter sides lighten towards white instead of saturating
    float reserved0 = 0.0f;     // padding to a whole vec4
    float reserved1 = 0.0f;
};
static_assert(sizeof(ShimmerUniforms) == 96, "ShimmerUniforms must match the shader's std140 block");

class ShaderMaterial
{
public:
    ~ShaderMaterial();

    // Uniform data is copied; safe to call every frame.
    bool setFragmentUniforms(Uint32 slot, const void* data, Uint32 size);
    bool setShimmerUniforms(const ShimmerUniforms& u);

    // Draw calls issued between bind() and unbind() use this material.
    bool bind(SDL_Renderer* pRenderer);
    static void unbind(SDL_Renderer* pRenderer);

private:
    friend class ShaderManager;
    ShaderMaterial() = default;
    ShaderMaterial(const ShaderMaterial&) = delete;
    ShaderMaterial& operator=(const ShaderMaterial&) = delete;

    SDL_GPURenderState* pState = nullptr;

    // last uniforms sent, identical data is skipped so sprites at rest keep batching
    ShimmerUniforms lastUniforms;
    bool bHasUniforms = false;
};