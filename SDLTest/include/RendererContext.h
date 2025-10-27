#pragma once
#include <SDL3/SDL.h>

class RendererContext
{
private: 
    SDL_Renderer* pRenderer;
public:
    SDL_Renderer* getRenderer();
/* * * * * * * * * * * * * * * * * * * * *
 *       SINGLETON-RELATED CONTENT       *
 * * * * * * * * * * * * * * * * * * * * */
private:
    static RendererContext* P_SHARED_INSTANCE;

private:
    RendererContext() {};
    RendererContext(const RendererContext&) {};
    RendererContext& operator=(const RendererContext&) {};

public:
    static void initialize(SDL_Renderer* pRenderer);
    static void destroy();

    static RendererContext* getInstance();
    /* * * * * * * * * * * * * * * * * * * * */
};

