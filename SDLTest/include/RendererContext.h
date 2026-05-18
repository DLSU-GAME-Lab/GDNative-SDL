#pragma once
#include <SDL3/SDL.h>
#include "Vector2D.h"

class RendererContext
{
private:
    SDL_Renderer* pRenderer;
public:
    SDL_Renderer* getRenderer();
    Vector2D getRenderScale() const;
/* * * * * * * * * * * * * * * * * * * * *
 *       SINGLETON-RELATED CONTENT       *
 * * * * * * * * * * * * * * * * * * * * */
private:
    static RendererContext* P_SHARED_INSTANCE;

private:
    RendererContext(SDL_Renderer* pRenderer);
    RendererContext(const RendererContext&) {};
    RendererContext& operator=(const RendererContext&) {};

public:
    static void initialize(SDL_Renderer* pRenderer);
    static void destroy();

    static RendererContext* getInstance();
    /* * * * * * * * * * * * * * * * * * * * */
};

