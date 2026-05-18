#include "RendererContext.h"
RendererContext* RendererContext::P_SHARED_INSTANCE = NULL;

RendererContext::RendererContext(SDL_Renderer* pRenderer) : pRenderer(pRenderer) {}

void RendererContext::initialize(SDL_Renderer* pRenderer)
{
    P_SHARED_INSTANCE = new RendererContext(pRenderer);
}
RendererContext* RendererContext::getInstance()
{
    return P_SHARED_INSTANCE;
}
void RendererContext::destroy()
{
    delete P_SHARED_INSTANCE;
}

SDL_Renderer* RendererContext::getRenderer()
{
    return this->pRenderer;
}

Vector2D RendererContext::getRenderScale() const
{
    float w, h;
    SDL_GetRenderScale(pRenderer, &w, &h);
    return Vector2D(w,h);
}
