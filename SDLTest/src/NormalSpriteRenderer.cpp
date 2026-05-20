// NormalSpriteRenderer.cpp
#include "NormalSpriteRenderer.h"
#include "AGameObject.h"
#include "ShimmerEffect.h"
#include "Settings.h"
#include <iostream>

// -------------------------------------------------------------------
// Constructor
// -------------------------------------------------------------------
NormalSpriteRenderer::NormalSpriteRenderer(
    const std::string& diffuseName,
    const std::string& normalName)
    : ARenderer("NormalSpriteRenderer"),
    strDiffuseName(diffuseName),
    strNormalName(normalName)
{
    mUniforms.colorR = 1.0f;
    mUniforms.colorG = 1.0f;
    mUniforms.colorB = 1.0f;
    mUniforms.colorA = 1.0f;
    mUniforms.time = 0.0f;
    mUniforms.phase = 0.0f;
    mUniforms.shimmerWidth = 0.15f;
    mUniforms.shimmerPower = 0.6f;
}

// -------------------------------------------------------------------
// Destructor
// -------------------------------------------------------------------
NormalSpriteRenderer::~NormalSpriteRenderer()
{
    SDL_GPUDevice* device = RendererContext::getInstance()->getGPUDevice();
    if (!device) return;

    if (pDiffuseGPU) {
        SDL_ReleaseGPUTexture(device, pDiffuseGPU);  pDiffuseGPU = nullptr;
    }
    if (pNormalGPU) {
        SDL_ReleaseGPUTexture(device, pNormalGPU);   pNormalGPU = nullptr;
    }
    if (pVertexBuffer) {
        SDL_ReleaseGPUBuffer(device, pVertexBuffer); pVertexBuffer = nullptr;
    }
    if (pSampler) {
        SDL_ReleaseGPUSampler(device, pSampler);     pSampler = nullptr;
    }
}

// -------------------------------------------------------------------
// uploadToGPU: converts SDL_Texture* to SDL_GPUTexture*
// -------------------------------------------------------------------
SDL_GPUTexture* NormalSpriteRenderer::uploadToGPU(SDL_Texture* pSDLTex)
{
    SDL_GPUDevice* device = RendererContext::getInstance()->getGPUDevice();
    SDL_Renderer* renderer = RendererContext::getInstance()->getRenderer();

    if (!device || !pSDLTex) return nullptr;

    float fw, fh;
    SDL_GetTextureSize(pSDLTex, &fw, &fh);
    int w = (int)fw;
    int h = (int)fh;

    // read pixels into a surface
    SDL_Surface* surface = SDL_CreateSurface(w, h, SDL_PIXELFORMAT_RGBA8888);
    if (!surface)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
            "[NormalSpriteRenderer] SDL_CreateSurface failed: %s", SDL_GetError());
        return nullptr;
    }

    // create GPU texture
    SDL_GPUTextureCreateInfo texInfo = {};
    texInfo.type = SDL_GPU_TEXTURETYPE_2D;
    texInfo.format = SDL_GPU_TEXTUREFORMAT_R8G8B8A8_UNORM;
    texInfo.usage = SDL_GPU_TEXTUREUSAGE_SAMPLER;
    texInfo.width = (Uint32)w;
    texInfo.height = (Uint32)h;
    texInfo.layer_count_or_depth = 1;
    texInfo.num_levels = 1;

    SDL_GPUTexture* pGPUTex = SDL_CreateGPUTexture(device, &texInfo);
    if (!pGPUTex)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
            "[NormalSpriteRenderer] SDL_CreateGPUTexture failed: %s", SDL_GetError());
        SDL_DestroySurface(surface);
        return nullptr;
    }

    // upload via transfer buffer
    Uint32 dataSize = w * h * 4;

    SDL_GPUTransferBufferCreateInfo transferInfo = {};
    transferInfo.usage = SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD;
    transferInfo.size = dataSize;

    SDL_GPUTransferBuffer* pTransfer = SDL_CreateGPUTransferBuffer(device, &transferInfo);
    if (!pTransfer)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
            "[NormalSpriteRenderer] SDL_CreateGPUTransferBuffer failed: %s", SDL_GetError());
        SDL_ReleaseGPUTexture(device, pGPUTex);
        SDL_DestroySurface(surface);
        return nullptr;
    }

    void* pMapped = SDL_MapGPUTransferBuffer(device, pTransfer, false);
    SDL_memcpy(pMapped, surface->pixels, dataSize);
    SDL_UnmapGPUTransferBuffer(device, pTransfer);
    SDL_DestroySurface(surface);

    SDL_GPUCommandBuffer* cmd = SDL_AcquireGPUCommandBuffer(device);
    SDL_GPUCopyPass* copyPass = SDL_BeginGPUCopyPass(cmd);

    SDL_GPUTextureTransferInfo src = {};
    src.transfer_buffer = pTransfer;
    src.offset = 0;
    src.pixels_per_row = (Uint32)w;
    src.rows_per_layer = (Uint32)h;

    SDL_GPUTextureRegion dst = {};
    dst.texture = pGPUTex;
    dst.w = (Uint32)w;
    dst.h = (Uint32)h;
    dst.d = 1;

    SDL_UploadToGPUTexture(copyPass, &src, &dst, false);
    SDL_EndGPUCopyPass(copyPass);
    SDL_SubmitGPUCommandBuffer(cmd);
    SDL_ReleaseGPUTransferBuffer(device, pTransfer);

    SDL_Log("[NormalSpriteRenderer] Uploaded GPU texture %dx%d", w, h);
    return pGPUTex;
}

// -------------------------------------------------------------------
// uploadQuad: builds screen-space quad into vertex buffer
// -------------------------------------------------------------------
void NormalSpriteRenderer::uploadQuad(SDL_FRect destRect)
{
    SDL_GPUDevice* device = RendererContext::getInstance()->getGPUDevice();
    if (!device) return;

    // convert screen pixels to NDC (-1 to 1)
    float x0 = (destRect.x / gameWidth) * 2.0f - 1.0f;
    float y0 = (destRect.y / gameHeight) * 2.0f - 1.0f;
    float x1 = ((destRect.x + destRect.w) / gameWidth) * 2.0f - 1.0f;
    float y1 = ((destRect.y + destRect.h) / gameHeight) * 2.0f - 1.0f;

    float u0 = bFlipX ? 1.0f : 0.0f;
    float u1 = bFlipX ? 0.0f : 1.0f;
    float v0 = bFlipY ? 1.0f : 0.0f;
    float v1 = bFlipY ? 0.0f : 1.0f;

    // two triangles: { x, y, u, v }
    float verts[24] = {
        x0, y0, u0, v0,
        x1, y0, u1, v0,
        x1, y1, u1, v1,

        x0, y0, u0, v0,
        x1, y1, u1, v1,
        x0, y1, u0, v1,
    };

    Uint32 dataSize = sizeof(verts);

    if (!pVertexBuffer)
    {
        SDL_GPUBufferCreateInfo bufInfo = {};
        bufInfo.usage = SDL_GPU_BUFFERUSAGE_VERTEX;
        bufInfo.size = dataSize;
        pVertexBuffer = SDL_CreateGPUBuffer(device, &bufInfo);
    }

    SDL_GPUTransferBufferCreateInfo transferInfo = {};
    transferInfo.usage = SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD;
    transferInfo.size = dataSize;

    SDL_GPUTransferBuffer* pTransfer = SDL_CreateGPUTransferBuffer(device, &transferInfo);
    void* pMapped = SDL_MapGPUTransferBuffer(device, pTransfer, false);
    SDL_memcpy(pMapped, verts, dataSize);
    SDL_UnmapGPUTransferBuffer(device, pTransfer);

    SDL_GPUCommandBuffer* cmd = SDL_AcquireGPUCommandBuffer(device);
    SDL_GPUCopyPass* copyPass = SDL_BeginGPUCopyPass(cmd);

    SDL_GPUTransferBufferLocation src = {};
    src.transfer_buffer = pTransfer;
    src.offset = 0;

    SDL_GPUBufferRegion dst = {};
    dst.buffer = pVertexBuffer;
    dst.offset = 0;
    dst.size = dataSize;

    SDL_UploadToGPUBuffer(copyPass, &src, &dst, false);
    SDL_EndGPUCopyPass(copyPass);
    SDL_SubmitGPUCommandBuffer(cmd);
    SDL_ReleaseGPUTransferBuffer(device, pTransfer);
}

// -------------------------------------------------------------------
// initialize
// -------------------------------------------------------------------
void NormalSpriteRenderer::initialize()
{
    SDL_GPUDevice* device = RendererContext::getInstance()->getGPUDevice();
    if (!device)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
            "[NormalSpriteRenderer] No GPU device available.");
        return;
    }

    // get pipeline from RendererContext
    pPipeline = RendererContext::getInstance()->getShaderPipeline("sprite");
    if (!pPipeline || !pPipeline->isValid())
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
            "[NormalSpriteRenderer] Shader pipeline not found.");
        return;
    }

    // load from TextureManager and upload to GPU
    SDL_Texture* pDiffuse = TextureManager::getInstance()->get(strDiffuseName);
    SDL_Texture* pNormal = TextureManager::getInstance()->get(strNormalName);

    if (!pDiffuse)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
            "[NormalSpriteRenderer] Diffuse texture not found: %s", strDiffuseName.c_str());
        return;
    }
    if (!pNormal)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
            "[NormalSpriteRenderer] Normal map not found: %s", strNormalName.c_str());
        return;
    }

    pDiffuseGPU = uploadToGPU(pDiffuse);
    pNormalGPU = uploadToGPU(pNormal);

    float fw, fh;
    SDL_GetTextureSize(pDiffuse, &fw, &fh);
    texSize = Vector2D(fw, fh);

    // create sampler
    SDL_GPUSamplerCreateInfo samplerInfo = {};
    samplerInfo.min_filter = SDL_GPU_FILTER_LINEAR;
    samplerInfo.mag_filter = SDL_GPU_FILTER_LINEAR;
    samplerInfo.address_mode_u = SDL_GPU_SAMPLERADDRESSMODE_CLAMP_TO_EDGE;
    samplerInfo.address_mode_v = SDL_GPU_SAMPLERADDRESSMODE_CLAMP_TO_EDGE;
    samplerInfo.address_mode_w = SDL_GPU_SAMPLERADDRESSMODE_CLAMP_TO_EDGE;
    pSampler = SDL_CreateGPUSampler(device, &samplerInfo);

    SDL_Log("[NormalSpriteRenderer] Initialized: diffuse=%s normal=%s",
        strDiffuseName.c_str(), strNormalName.c_str());
}

void NormalSpriteRenderer::onAttach()
{
    initialize();
}

// -------------------------------------------------------------------
// perform
// -------------------------------------------------------------------
void NormalSpriteRenderer::perform()
{
    if (!pPipeline || !pPipeline->isValid()) return;
    if (!pDiffuseGPU || !pNormalGPU)         return;
    if (!pVertexBuffer)                       return;

    SDL_GPUDevice* device = RendererContext::getInstance()->getGPUDevice();
    if (!device) return;

    AGameObject* owner = this->getOwner();
    Camera* pCam = CameraManager::getInstance()->getCurrentCamera();

    Vector2D scale = owner->getScale();
    Vector2D size = texSize * scale;
    Vector2D pos = owner->getPos();
    pos -= size * pivot;

    SDL_FRect destRect = { pos.x, pos.y, size.x, size.y };

    if (!owner->getIsScreenObject())
        destRect = pCam->worldToScreenRect(destRect);

    if (!inCameraView(destRect)) return;

    // read shimmer phase from ShimmerEffect component if present
    float phase = 0.0f;
    ShimmerEffect* pShimmer = (ShimmerEffect*)owner->findComponentByName("ShimmerEffect");
    if (pShimmer) phase = pShimmer->getPhase();
    mUniforms.phase = phase;

    fElapsedTime += 0.016f;
    mUniforms.time = fElapsedTime;

    uploadQuad(destRect);

    SDL_GPUCommandBuffer* cmd = SDL_AcquireGPUCommandBuffer(device);

    SDL_GPUTexture* swapchainTex = nullptr;
    Uint32 swapW, swapH;
    if (!SDL_WaitAndAcquireGPUSwapchainTexture(cmd,
        RendererContext::getInstance()->getWindow(),
        &swapchainTex, &swapW, &swapH))
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
            "[NormalSpriteRenderer] Failed to acquire swapchain: %s", SDL_GetError());
        SDL_CancelGPUCommandBuffer(cmd);
        return;
    }

    SDL_GPUColorTargetInfo colorTarget = {};
    colorTarget.texture = swapchainTex;
    colorTarget.load_op = SDL_GPU_LOADOP_LOAD;
    colorTarget.store_op = SDL_GPU_STOREOP_STORE;
    colorTarget.clear_color = { 0, 0, 0, 1 };

    SDL_GPURenderPass* pass = SDL_BeginGPURenderPass(cmd, &colorTarget, 1, nullptr);

    pPipeline->bind(pass);

    SDL_GPUBufferBinding vertBind = {};
    vertBind.buffer = pVertexBuffer;
    vertBind.offset = 0;
    SDL_BindGPUVertexBuffers(pass, 0, &vertBind, 1);

    SDL_GPUTextureSamplerBinding texBinds[2] = {};
    texBinds[0].texture = pDiffuseGPU;
    texBinds[0].sampler = pSampler;
    texBinds[1].texture = pNormalGPU;
    texBinds[1].sampler = pSampler;
    SDL_BindGPUFragmentSamplers(pass, 0, texBinds, 2);

    pPipeline->pushUniforms(cmd, mUniforms);

    SDL_DrawGPUPrimitives(pass, 6, 1, 0, 0);

    SDL_EndGPURenderPass(pass);
    SDL_SubmitGPUCommandBuffer(cmd);
}

// -------------------------------------------------------------------
// drawWidget
// -------------------------------------------------------------------
void NormalSpriteRenderer::drawWidget()
{
    AGameObject* owner = this->getOwner();
    Camera* pCam = CameraManager::getInstance()->getCurrentCamera();

    Vector2D scale = owner->getScale();
    Vector2D size = texSize * scale;
    Vector2D pos = owner->getPos();
    pos -= size * pivot;

    SDL_FRect destRect = { pos.x, pos.y, size.x, size.y };
    destRect = pCam->worldToScreenRect(destRect);

    SDL_SetRenderDrawBlendMode(pRenderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(pRenderer, 0, 255, 255, 127);
    SDL_RenderRect(pRenderer, &destRect);
}

// -------------------------------------------------------------------
// setColor
// -------------------------------------------------------------------
void NormalSpriteRenderer::setColor(float r, float g, float b, float a)
{
    mUniforms.colorR = r;
    mUniforms.colorG = g;
    mUniforms.colorB = b;
    mUniforms.colorA = a;
}
void NormalSpriteRenderer::setFlipX(bool flip)
{
    this->bFlipX = flip;
}
void NormalSpriteRenderer::setFlipY(bool flip)
{
    this->bFlipY = flip;
}
void NormalSpriteRenderer::setPivot(Vector2D pivot)
{
    this->pivot = Vector2D(SDL_clamp(pivot.x, 0, 1), SDL_clamp(pivot.y, 0, 1));
}