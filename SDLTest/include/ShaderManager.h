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
#include <memory>

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

class ShaderManager
{
public:
    bool isSupported() const { return bSupported; }

    // Shimmer material for a normal map registered in TextureManager under
    // `normalMapKey`. An empty key uses a built-in flat normal (plain glint).
    // Returns nullptr if unsupported or the named texture doesn't exist.
    // Do not cache the pointer across frames/scene loads; look it up each time.
    ShaderMaterial* getShimmerMaterial(const std::string& normalMapKey);

    // TextureManager calls this right before destroying a texture, so no
    // material keeps a dangling GPU texture.
    void onTextureDestroyed(SDL_Texture* pTexture);

private:
    SDL_Renderer* pRenderer = nullptr;
    SDL_GPUDevice* pDevice = nullptr;      // owned by the renderer; never destroy
    SDL_GPUShader* pShimmerShader = nullptr;
    SDL_GPUSampler* pLinearSampler = nullptr;
    SDL_Texture* pFlatNormal = nullptr;  // 1x1 (0.5, 0.5, 1.0)
    bool            bSupported = false;

    std::unordered_map<SDL_Texture*, std::unique_ptr<ShaderMaterial>> mapShimmer;

    bool init(SDL_Renderer* pRenderer);
    void shutdown();
    SDL_GPUShader* loadFragmentShader(const std::string& path, int numSamplers, int numUniformBuffers);
    ShaderMaterial* createShimmerMaterial(SDL_Texture* pNormalTexture);

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