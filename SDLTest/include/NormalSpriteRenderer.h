// NormalSpriteRenderer.h
#pragma once
#include "ARenderer.h"
#include "ShaderPipeline.h"
#include "TextureManager.h"
#include "RendererContext.h"
#include "CameraManager.h"
#include "ShimmerEffect.h"
#include <string>

class NormalSpriteRenderer : public ARenderer
{
public:
    
    NormalSpriteRenderer(const std::string& diffuseName, const std::string& normalName);
    ~NormalSpriteRenderer();

    void initialize();
    void perform()    override;
    void drawWidget() override;
    void onAttach()   override;

    void setColor(float r, float g, float b, float a);
    void setFlipX(bool flip); 
    void setFlipY(bool flip); 
    void setPivot(Vector2D pivot);
    SDL_FRect getRect() const override;
private:
    // texture names
    std::string strDiffuseName;
    std::string strNormalName;
    //destRect
    SDL_FRect mDestRect = {};
    // GPU textures
    SDL_GPUTexture* pDiffuseGPU = nullptr;
    SDL_GPUTexture* pNormalGPU = nullptr;

    // GPU buffers
    SDL_GPUBuffer* pVertexBuffer = nullptr;
    SDL_GPUSampler* pSampler = nullptr;

    // pipeline (owned by RendererContext)
    ShaderPipeline* pPipeline = nullptr;

    // per-draw uniforms
    ShaderUniforms  mUniforms = {};

    // transform
    Vector2D pivot = { 0.5f, 0.5f };
    Vector2D texSize = { 0.0f, 0.0f };
    bool     bFlipX = false;
    bool     bFlipY = false;
    float    fElapsedTime = 0.0f;

    // helpers
    SDL_GPUTexture* uploadToGPU(SDL_Texture* pSDLTex);
    void            uploadQuad(SDL_FRect destRect);
};