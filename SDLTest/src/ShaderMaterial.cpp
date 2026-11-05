#include "ShaderMaterial.h"
#include <cstring>

ShaderMaterial::~ShaderMaterial()
{
    if (pState) SDL_DestroyGPURenderState(pState);
}

bool ShaderMaterial::setFragmentUniforms(Uint32 slot, const void* data, Uint32 size)
{
    return pState && SDL_SetGPURenderStateFragmentUniforms(pState, slot, data, size);
}

// Every uniform change makes SDL flush the queued draws, so unchanged data is skipped.
bool ShaderMaterial::setShimmerUniforms(const ShimmerUniforms& u)
{
    if (bHasUniforms && std::memcmp(&lastUniforms, &u, sizeof(u)) == 0) return true;
    if (!setFragmentUniforms(0, &u, sizeof(u))) return false;

    lastUniforms = u;
    bHasUniforms = true;
    return true;
}

bool ShaderMaterial::bind(SDL_Renderer* pRenderer)
{
    return pState && SDL_SetGPURenderState(pRenderer, pState);
}

void ShaderMaterial::unbind(SDL_Renderer* pRenderer)
{
    SDL_SetGPURenderState(pRenderer, NULL);
}