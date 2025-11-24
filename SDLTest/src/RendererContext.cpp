#include "RendererContext.h"
RendererContext* RendererContext::P_SHARED_INSTANCE = NULL;

RendererContext::RendererContext(SDL_Renderer* pRenderer)
{
    this->pRenderer = pRenderer;
}

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
