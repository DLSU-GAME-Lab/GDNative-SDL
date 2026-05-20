// RendererContext.cpp
#include "RendererContext.h"
#include <iostream>

RendererContext* RendererContext::P_SHARED_INSTANCE = NULL;

RendererContext::RendererContext(
    SDL_Renderer* pRenderer,
    SDL_GPUDevice* pGPUDevice,
    SDL_Window* pWindow)
{
    this->pRenderer = pRenderer;
    this->pGPUDevice = pGPUDevice;
    this->pWindow = pWindow;
}

void RendererContext::initialize(
    SDL_Renderer* pRenderer,
    SDL_GPUDevice* pGPUDevice,
    SDL_Window* pWindow)
{
    P_SHARED_INSTANCE = new RendererContext(pRenderer, pGPUDevice, pWindow);
}

RendererContext* RendererContext::getInstance()
{
    return P_SHARED_INSTANCE;
}

void RendererContext::destroy()
{
    // destroy all registered pipelines
    for (auto& pair : P_SHARED_INSTANCE->mapPipelines)
    {
        delete pair.second;
    }
    P_SHARED_INSTANCE->mapPipelines.clear();

    delete P_SHARED_INSTANCE;
    P_SHARED_INSTANCE = nullptr;
}

void RendererContext::render()
{
    const SDL_GPUColorTargetInfo colorTargetInfo = {};
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

SDL_Window* RendererContext::getWindow()
{
    return this->pWindow;
}

void RendererContext::registerShaderPipeline(const std::string& name, ShaderPipeline* pipeline)
{
    if (mapPipelines.find(name) != mapPipelines.end())
    {
        SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION,
            "[RendererContext] Pipeline '%s' already registered, replacing.", name.c_str());
        delete mapPipelines[name];
    }
    mapPipelines[name] = pipeline;
    SDL_Log("[RendererContext] Registered pipeline '%s'", name.c_str());
}

ShaderPipeline* RendererContext::getShaderPipeline(const std::string& name)
{
    auto it = mapPipelines.find(name);
    if (it == mapPipelines.end())
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
            "[RendererContext] Pipeline '%s' not found.", name.c_str());
        return nullptr;
    }
    return it->second;
}