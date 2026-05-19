// ShaderPipeline.h
#pragma once
#include <SDL3/SDL.h>
#include <SDL3/SDL_gpu.h>
#include <string>

// All per-draw data pushed to both shader stages.
// Must stay under 128 bytes — Vulkan minimum guarantee.
struct ShaderUniforms
{
    // vertex stage
    float posX, posY;      // position in NDC (-1 to 1)
    float sizeX, sizeY;     // size in NDC

    // fragment stage
    float colorR, colorG, colorB, colorA;  // tint
    float time;                            // elapsed seconds
    float phase;                           // shimmer sweep (0.0 - 1.0)
    float shimmerWidth;                    // band width
    float shimmerPower;                    // intensity
};
static_assert(sizeof(ShaderUniforms) <= 128,
    "ShaderUniforms exceeds Vulkan push constant minimum guarantee.");

class ShaderPipeline
{
public:
    ShaderPipeline(SDL_GPUDevice* device, const char* vertSpvPath, const char* fragSpvPath);
    ~ShaderPipeline();

    // no copy — owns GPU resources
    ShaderPipeline(const ShaderPipeline&) = delete;
    ShaderPipeline& operator=(const ShaderPipeline&) = delete;

    void bind(SDL_GPURenderPass* pass);
    void pushUniforms(SDL_GPUCommandBuffer* cmd, const ShaderUniforms& u);
    bool isValid() const;

private:
    SDL_GPUDevice* pDevice;    // non-owning
    SDL_GPUGraphicsPipeline* pPipeline;
};