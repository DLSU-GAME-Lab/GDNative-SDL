#include "ShaderManager.h"
#include "TextureManager.h"
#include <cstring>

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

    // Slot 0 = the sprite SDL draws, slot 1 = normal map, slot 2 = mask. One uniform block.
    pShimmerShader = loadFragmentShader("shaders/sprite_shimmer.frag.spv", 3, 1);
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

    // White mask for sprites with no mask, so the whole sprite can shimmer.
    const Uint8 white[4] = { 255, 255, 255, 255 };
    pFlatMask = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STATIC, 1, 1);
    if (!pFlatMask || !SDL_UpdateTexture(pFlatMask, NULL, white, 4))
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "[ShaderManager] flat mask texture failed: %s", SDL_GetError());
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

ShaderMaterial* ShaderManager::createShimmerMaterial(SDL_Texture* pNormalTexture, SDL_Texture* pMaskTexture)
{
    SDL_GPUTexture* gpuNormal = (SDL_GPUTexture*)SDL_GetPointerProperty(
        SDL_GetTextureProperties(pNormalTexture), SDL_PROP_TEXTURE_GPU_TEXTURE_POINTER, NULL);
    SDL_GPUTexture* gpuMask = (SDL_GPUTexture*)SDL_GetPointerProperty(
        SDL_GetTextureProperties(pMaskTexture), SDL_PROP_TEXTURE_GPU_TEXTURE_POINTER, NULL);
    if (!gpuNormal || !gpuMask)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "[ShaderManager] Texture has no GPU texture pointer");
        return nullptr;
    }

    // bound after the sprite SDL draws: [0] = slot 1 (normal map), [1] = slot 2 (mask)
    SDL_GPUTextureSamplerBinding bindings[2];
    SDL_zero(bindings);
    bindings[0].texture = gpuNormal;
    bindings[0].sampler = pLinearSampler;
    bindings[1].texture = gpuMask;
    bindings[1].sampler = pLinearSampler;

    SDL_GPURenderStateCreateInfo ci;
    SDL_zero(ci);
    ci.fragment_shader = pShimmerShader;
    ci.num_sampler_bindings = 2;
    ci.sampler_bindings = bindings;

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

ShaderMaterial* ShaderManager::getShimmerMaterial(const std::string& normalMapKey, const std::string& maskKey)
{
    if (!bSupported) return nullptr;

    SDL_Texture* normalTex = pFlatNormal;
    if (!normalMapKey.empty())
    {
        normalTex = TextureManager::getInstance()->get(normalMapKey);
        if (!normalTex) return nullptr;     // missing normal map: caller draws plain
    }

    SDL_Texture* maskTex = pFlatMask;
    if (!maskKey.empty())
    {
        maskTex = TextureManager::getInstance()->get(maskKey);
        if (!maskTex) return nullptr;       // missing mask: caller draws plain
    }

    auto key = std::make_pair(normalTex, maskTex);
    auto it = mapShimmer.find(key);
    if (it != mapShimmer.end()) return it->second.get();

    ShaderMaterial* mat = createShimmerMaterial(normalTex, maskTex);
    if (!mat) return nullptr;
    mapShimmer[key].reset(mat);
    return mat;
}

void ShaderManager::onTextureDestroyed(SDL_Texture* pTexture)
{
    // destroys every render state that references it, as the normal map or the mask
    for (auto it = mapShimmer.begin(); it != mapShimmer.end(); )
    {
        if (it->first.first == pTexture || it->first.second == pTexture)
            it = mapShimmer.erase(it);
        else
            ++it;
    }
}

void ShaderManager::shutdown()
{
    if (pDevice) SDL_SetGPURenderState(pRenderer, NULL);   // only a GPU renderer has a render state
    mapShimmer.clear();
    if (pDevice)
    {
        if (pLinearSampler) SDL_ReleaseGPUSampler(pDevice, pLinearSampler);
        if (pShimmerShader) SDL_ReleaseGPUShader(pDevice, pShimmerShader);
    }
    if (pFlatNormal) SDL_DestroyTexture(pFlatNormal);
    if (pFlatMask) SDL_DestroyTexture(pFlatMask);
    pLinearSampler = nullptr;
    pShimmerShader = nullptr;
    pFlatNormal = nullptr;
    pFlatMask = nullptr;
}

/* * * * * * * * * * * * * * * * * * * * *
 *       SINGLETON-RELATED CONTENT       *
 * * * * * * * * * * * * * * * * * * * * */
ShaderManager* ShaderManager::P_SHARED_INSTANCE = NULL;

void ShaderManager::initialize(SDL_Renderer* pRenderer)
{
    P_SHARED_INSTANCE = new ShaderManager();
    P_SHARED_INSTANCE->bSupported = P_SHARED_INSTANCE->init(pRenderer);
    if (!P_SHARED_INSTANCE->bSupported) P_SHARED_INSTANCE->shutdown();   // release whatever init() created before failing
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