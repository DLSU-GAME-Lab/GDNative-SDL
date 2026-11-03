#pragma once
// ---------------------------------------------------------------------------
// ShaderManager: owns custom fragment shaders + SDL_GPURenderState "materials"
// for the SDL GPU renderer (SDL 3.4+).
//
// If the renderer is NOT a GPU renderer (creation failed / old Android device
// without Vulkan), isSupported() is false and callers must use their CPU
// fallback. Nothing here is ever required for the game to run.
// ---------------------------------------------------------------------------
#include <SDL3/SDL.h>
#include <string>
#include <unordered_map>
#include <map>
#include <utility>
#include <memory>
#include "ShaderMaterial.h"

class ShaderManager
{
public:
    bool isSupported() const { return bSupported; }

    // Shimmer material for a normal map and a mask registered in TextureManager
    // under `normalMapKey` / `maskKey`. The mask uses its red channel and limits
    // where the glint shows. An empty normal key uses a built-in flat normal
    // (plain glint), an empty mask key uses a built-in white mask (whole sprite).
    // Returns nullptr if unsupported or a named texture doesn't exist.
    // Do not cache the pointer across frames/scene loads; look it up each time.
    ShaderMaterial* getShimmerMaterial(const std::string& normalMapKey, const std::string& maskKey = "");

    // TextureManager calls this right before destroying a texture, so no
    // material keeps a dangling GPU texture.
    void onTextureDestroyed(SDL_Texture* pTexture);

private:
    SDL_Renderer* pRenderer = nullptr;
    SDL_GPUDevice* pDevice = nullptr;      // owned by the renderer; never destroy
    SDL_GPUShader* pShimmerShader = nullptr;
    SDL_GPUSampler* pLinearSampler = nullptr;
    SDL_Texture* pFlatNormal = nullptr;  // 1x1 (0.5, 0.5, 1.0)
    SDL_Texture* pFlatMask = nullptr;    // 1x1 white
    bool            bSupported = false;

    // one material per (normal map, mask) pair
    std::map<std::pair<SDL_Texture*, SDL_Texture*>, std::unique_ptr<ShaderMaterial>> mapShimmer;

    bool init(SDL_Renderer* pRenderer);
    void shutdown();
    SDL_GPUShader* loadFragmentShader(const std::string& path, int numSamplers, int numUniformBuffers);
    ShaderMaterial* createShimmerMaterial(SDL_Texture* pNormalTexture, SDL_Texture* pMaskTexture);

    /* * * * * * * * * * * * * * * * * * * * *
     *       SINGLETON-RELATED CONTENT       *
     * * * * * * * * * * * * * * * * * * * * */
private:
    static ShaderManager* P_SHARED_INSTANCE;

private:
    ShaderManager() {}
    ShaderManager(const ShaderManager&) = delete;
    ShaderManager& operator=(const ShaderManager&) = delete;

public:
    static void initialize(SDL_Renderer* pRenderer);
    static void destroy();
    static ShaderManager* getInstance();
    /* * * * * * * * * * * * * * * * * * * * */
};