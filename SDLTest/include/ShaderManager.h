#pragma once
// ---------------------------------------------------------------------------
// ShaderManager: owns custom fragment shaders + SDL_GPURenderState "materials"
// for the SDL GPU renderer (SDL 3.4+).
//
// Game-independent: nothing in here knows about a game's art, scenes or gems.
// Shaders are opt-in per sprite, so a scene that never asks for one pays
// nothing (the SPIR-V is only loaded the first time a material is requested).
//
// If the renderer is NOT a GPU renderer (creation failed / old Android device
// without Vulkan), isSupported() is false and callers must draw plain. Nothing
// here is ever required for the game to run.
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
    // Creates the renderer for a window: the GPU renderer when bUseGPU is set
    // (required for custom shaders), the normal SDL renderer otherwise or when
    // the GPU renderer can't be created. Replaces the renderer block in Runner.
    static SDL_Renderer* createRenderer(SDL_Window* pWindow, bool bUseGPU);

    // Naming convention shared by every game: a texture loaded as "<name>" finds
    // its normal map as "<name>_Normal" and its mask as "<name>_Mask".
    static const char* normalSuffix() { return "_Normal"; }
    static const char* maskSuffix() { return "_Mask"; }

    bool isSupported() const { return bSupported && bEnabled && !bShimmerFailed; }

    // Lets a scene switch shaders off (and on again): sprites are drawn plain
    // while disabled. Does not free anything.
    void setEnabled(bool bEnabled) { this->bEnabled = bEnabled; }

    // Style used by every sprite that did not set its own, so a game or a scene
    // tunes the look in one place.
    void setDefaultStyle(const ShimmerUniforms& style) { this->defaultStyle = style; }
    const ShimmerUniforms& getDefaultStyle() const { return defaultStyle; }

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
    bool            bSupported = false;       // GPU renderer with SPIR-V support
    bool            bEnabled = true;          // scene-level switch
    bool            bShimmerLoaded = false;   // shader + helper textures created
    bool            bShimmerFailed = false;   // loading failed once, not retried
    ShimmerUniforms defaultStyle;

    // one material per (normal map, mask) pair
    std::map<std::pair<SDL_Texture*, SDL_Texture*>, std::unique_ptr<ShaderMaterial>> mapShimmer;

    bool init(SDL_Renderer* pRenderer);
    bool loadShimmerResources();
    void releaseResources();
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