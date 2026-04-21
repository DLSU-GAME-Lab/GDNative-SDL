#include "RendererContext.h"

void RendererContext::render()
{
    const SDL_GPUColorTargetInfo colorTargetInfo =
    {

    };

    SDL_GPURenderPass* pRenderPass = SDL_BeginGPURenderPass(
        SDL_AcquireGPUCommandBuffer(this->pGPUDevice),
		&colorTargetInfo,
        0,
        NULL
    );

	SDL_BindGPUGraphicsPipeline(pRenderPass, NULL);

	SDL_EndGPURenderPass(pRenderPass);
}

SDL_Renderer* RendererContext::getRenderer()
{
    return this->pRenderer;
}

SDL_GPUDevice* RendererContext::getGPUDevice()
{
    return this->pGPUDevice;
}

RendererContext* RendererContext::P_SHARED_INSTANCE = NULL;

RendererContext::RendererContext(SDL_Renderer* pRenderer, SDL_GPUDevice* pGPUDevice)
{
    this->pRenderer = pRenderer;
	this->pGPUDevice = pGPUDevice;
}

void RendererContext::initialize(SDL_Renderer* pRenderer, SDL_GPUDevice* pGPUDevice)
{
    P_SHARED_INSTANCE = new RendererContext(pRenderer, pGPUDevice);
}

RendererContext* RendererContext::getInstance()
{
    return P_SHARED_INSTANCE;
}

void RendererContext::destroy()
{
    delete P_SHARED_INSTANCE;
}
