// RendererContext.h
#pragma once
#include <SDL3/SDL.h>
#include <SDL3/SDL_gpu.h>
#include <string>
#include <unordered_map>
#include "ShaderPipeline.h"

class RendererContext
{
public:
    static void initialize(SDL_Renderer* pRenderer, SDL_GPUDevice* pGPUDevice, SDL_Window* pWindow);
    static RendererContext* getInstance();
    static void             destroy();

    void render();

    // getters
    SDL_Renderer* getRenderer();
    SDL_GPUDevice* getGPUDevice();
    SDL_Window* getWindow();

    // shader pipeline registry
    void            registerShaderPipeline(const std::string& name, ShaderPipeline* pipeline);
    ShaderPipeline* getShaderPipeline(const std::string& name);

private:
    RendererContext(SDL_Renderer* pRenderer, SDL_GPUDevice* pGPUDevice, SDL_Window* pWindow);

    SDL_Renderer* pRenderer = nullptr;
    SDL_GPUDevice* pGPUDevice = nullptr;
    SDL_Window* pWindow = nullptr;

    std::unordered_map<std::string, ShaderPipeline*> mapPipelines;

    static RendererContext* P_SHARED_INSTANCE;
};