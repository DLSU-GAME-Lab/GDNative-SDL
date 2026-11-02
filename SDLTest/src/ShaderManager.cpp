#include "ShaderManager.h"
#include "TextureManager.h"
#include <cstring>

// ------------------------------- ShaderMaterial -----------------------------
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

// ------------------------------- ShaderManager ------------------------------
static std::string resolveAssetPath(std::string path)
{
#ifdef __ANDROID__
    // Android: SDL reads from the APK's assets folder; no "Assets/" prefix.
    if (path.rfind("Assets/", 0) == 0) path = path.substr(strlen("Assets/"));
#else
    if (path.rfind("Assets/", 0) != 0) path = "Assets/" + path;
#endif
    return path;
}

bool ShaderManager::init(SDL_Renderer* renderer)
{
    pRenderer = renderer;
    pDevice = SDL_GetGPURendererDevice(renderer);   // NULL if not a GPU renderer
    if (!pDevice)
    {
        SDL_Log("[ShaderManager] Not a GPU renderer - custom shaders disabled.");
        return false;
    }

    if (!(SDL_GetGPUShaderFormats(pDevice) & SDL_GPU_SHADERFORMAT_SPIRV))
    {
        SDL_Log("[ShaderManager] GPU device has no SPIR-V support - custom shaders disabled.");
        return false;
    }

    // Slot 0 = the sprite SDL draws, slot 1 = normal map. One uniform block.
    pShimmerShader = loadFragmentShader("shaders/sprite_shimmer.frag.spv", 2, 1);
    if (!pShimmerShader) return false;

    SDL_GPUSamplerCreateInfo si;
    SDL_zero(si);
    si.min_filter = SDL_GPU_FILTER_LINEAR;
    si.mag_filter = SDL_GPU_FILTER_LINEAR;
    si.mipmap_mode = SDL_GPU_SAMPLERMIPMAPMODE_NEAREST;
    si.address_mode_u = SDL_GPU_SAMPLERADDRESSMODE_CLAMP_TO_EDGE;
    si.address_mode_v = SDL_GPU_SAMPLERADDRESSMODE_CLAMP_TO_EDGE;
    si.address_mode_w = SDL_GPU_SAMPLERADDRESSMODE_CLAMP_TO_EDGE;
    pLinearSampler = SDL_CreateGPUSampler(pDevice, &si);
    if (!pLinearSampler)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "[ShaderManager] SDL_CreateGPUSampler failed: %s", SDL_GetError());
        return false;
    }

    // Flat normal (pointing straight at the viewer) for sprites with no normal map.
    const Uint8 flat[4] = { 128, 128, 255, 255 };
    pFlatNormal = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STATIC, 1, 1);
    if (!pFlatNormal || !SDL_UpdateTexture(pFlatNormal, NULL, flat, 4))
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "[ShaderManager] flat normal texture failed: %s", SDL_GetError());
        return false;
    }

    SDL_Log("[ShaderManager] Custom shaders enabled (SPIR-V).");
    return true;
}

SDL_GPUShader* ShaderManager::loadFragmentShader(const std::string& path, int numSamplers, int numUniformBuffers)
{
    std::string resolved = resolveAssetPath(path);

    size_t size = 0;
    void* code = SDL_LoadFile(resolved.c_str(), &size);   // reads APK assets on Android too
    if (!code)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "[ShaderManager] Can't load '%s': %s", resolved.c_str(), SDL_GetError());
        return nullptr;
    }

    SDL_GPUShaderCreateInfo info;
    SDL_zero(info);
    info.code = static_cast<const Uint8*>(code);
    info.code_size = size;
    info.entrypoint = "main";
    info.format = SDL_GPU_SHADERFORMAT_SPIRV;
    info.stage = SDL_GPU_SHADERSTAGE_FRAGMENT;
    info.num_samplers = numSamplers;
    info.num_uniform_buffers = numUniformBuffers;

    SDL_GPUShader* shader = SDL_CreateGPUShader(pDevice, &info);
    SDL_free(code);
    if (!shader)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "[ShaderManager] SDL_CreateGPUShader('%s') failed: %s", resolved.c_str(), SDL_GetError());
    }
    return shader;
}

ShaderMaterial* ShaderManager::createShimmerMaterial(SDL_Texture* pNormalTexture)
{
    SDL_GPUTexture* gpuTex = (SDL_GPUTexture*)SDL_GetPointerProperty(
        SDL_GetTextureProperties(pNormalTexture), SDL_PROP_TEXTURE_GPU_TEXTURE_POINTER, NULL);
    if (!gpuTex)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "[ShaderManager] Texture has no GPU texture pointer");
        return nullptr;
    }

    SDL_GPUTextureSamplerBinding binding;
    SDL_zero(binding);
    binding.texture = gpuTex;
    binding.sampler = pLinearSampler;

    SDL_GPURenderStateCreateInfo ci;
    SDL_zero(ci);
    ci.fragment_shader = pShimmerShader;
    ci.num_sampler_bindings = 1;
    ci.sampler_bindings = &binding;

    SDL_GPURenderState* state = SDL_CreateGPURenderState(pRenderer, &ci);
    if (!state)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "[ShaderManager] SDL_CreateGPURenderState failed: %s", SDL_GetError());
        return nullptr;
    }

    ShaderMaterial* mat = new ShaderMaterial();
    mat->pState = state;
    mat->setShimmerUniforms(ShimmerUniforms());   // sane defaults until the first real update
    return mat;
}

ShaderMaterial* ShaderManager::getShimmerMaterial(const std::string& normalMapKey)
{
    if (!bSupported) return nullptr;

    SDL_Texture* normalTex = pFlatNormal;
    if (!normalMapKey.empty())
    {
        normalTex = TextureManager::getInstance()->get(normalMapKey);
        if (!normalTex) return nullptr;     // missing normal map: caller draws plain
    }

    auto it = mapShimmer.find(normalTex);
    if (it != mapShimmer.end()) return it->second.get();

    ShaderMaterial* mat = createShimmerMaterial(normalTex);
    if (!mat) return nullptr;
    mapShimmer[normalTex].reset(mat);
    return mat;
}

void ShaderManager::onTextureDestroyed(SDL_Texture* pTexture)
{
    mapShimmer.erase(pTexture);   // destroys the render state that references it
}

void ShaderManager::shutdown()
{
    SDL_SetGPURenderState(pRenderer, NULL);
    mapShimmer.clear();
    if (pDevice)
    {
        if (pLinearSampler) SDL_ReleaseGPUSampler(pDevice, pLinearSampler);
        if (pShimmerShader) SDL_ReleaseGPUShader(pDevice, pShimmerShader);
    }
    if (pFlatNormal) SDL_DestroyTexture(pFlatNormal);
    pLinearSampler = nullptr;
    pShimmerShader = nullptr;
    pFlatNormal = nullptr;
}

/* * * * * * * * * * * * * * * * * * * * *
 *       SINGLETON-RELATED CONTENT       *
 * * * * * * * * * * * * * * * * * * * * */
ShaderManager* ShaderManager::P_SHARED_INSTANCE = NULL;

void ShaderManager::initialize(SDL_Renderer* pRenderer)
{
    P_SHARED_INSTANCE = new ShaderManager();
    P_SHARED_INSTANCE->bSupported = P_SHARED_INSTANCE->init(pRenderer);
}

void ShaderManager::destroy()
{
    if (!P_SHARED_INSTANCE) return;
    P_SHARED_INSTANCE->shutdown();
    delete P_SHARED_INSTANCE;
    P_SHARED_INSTANCE = NULL;
}

ShaderManager* ShaderManager::getInstance()
{
    return P_SHARED_INSTANCE;
}