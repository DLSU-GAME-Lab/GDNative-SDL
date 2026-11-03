#pragma once
#include <SDL3/SDL.h>

// Must match `ShimmerParams` in shaders/sprite_shimmer.frag (std140, 48 bytes).
struct ShimmerUniforms
{
    // vec4 a
    float phase = 0.0f;   // 0..1, driven by ShimmerEffect
    float intensity = 0.6f;   // glint brightness
    float shininess = 24.0f;  // specular exponent (higher = tighter highlight)
    float bandWidth = 0.30f;  // gaussian width of the sweep, in uv units
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
};
static_assert(sizeof(ShimmerUniforms) == 48, "ShimmerUniforms must match the shader's std140 block");

class ShaderMaterial
{
public:
    ~ShaderMaterial();

    // Uniform data is copied; safe to call every frame.
    bool setFragmentUniforms(Uint32 slot, const void* data, Uint32 size);
    bool setShimmerUniforms(const ShimmerUniforms& u) { return setFragmentUniforms(0, &u, sizeof(u)); }

    // Draw calls issued between bind() and unbind() use this material.
    bool bind(SDL_Renderer* pRenderer);
    static void unbind(SDL_Renderer* pRenderer);

private:
    friend class ShaderManager;
    ShaderMaterial() = default;
    ShaderMaterial(const ShaderMaterial&) = delete;
    ShaderMaterial& operator=(const ShaderMaterial&) = delete;

    SDL_GPURenderState* pState = nullptr;
};
