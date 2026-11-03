#include "ShaderMaterial.h"

ShaderMaterial::~ShaderMaterial()
{
    if (pState) SDL_DestroyGPURenderState(pState);
}

bool ShaderMaterial::setFragmentUniforms(Uint32 slot, const void* data, Uint32 size)
{
    return pState && SDL_SetGPURenderStateFragmentUniforms(pState, slot, data, size);
}

bool ShaderMaterial::bind(SDL_Renderer* pRenderer)
{
    return pState && SDL_SetGPURenderState(pRenderer, pState);
}

void ShaderMaterial::unbind(SDL_Renderer* pRenderer)
{
    SDL_SetGPURenderState(pRenderer, NULL);
}
