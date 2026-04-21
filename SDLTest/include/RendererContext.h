#pragma once
#include <SDL3/SDL.h>

class RendererContext
{
private: 
    SDL_Renderer* pRenderer;
	SDL_GPUDevice* pGPUDevice;

public:
    void render();

    SDL_Renderer* getRenderer();
	SDL_GPUDevice* getGPUDevice();
/* * * * * * * * * * * * * * * * * * * * *
 *       SINGLETON-RELATED CONTENT       *
 * * * * * * * * * * * * * * * * * * * * */
private:
    static RendererContext* P_SHARED_INSTANCE;

private:
    RendererContext(SDL_Renderer* pRenderer, SDL_GPUDevice* pGPUDevice);
    RendererContext(const RendererContext&) {};
    RendererContext& operator=(const RendererContext&) {};

public:
    static void initialize(SDL_Renderer* pRenderer, SDL_GPUDevice* pGPUDevice);
    static void destroy();

    static RendererContext* getInstance();
    /* * * * * * * * * * * * * * * * * * * * */
};

