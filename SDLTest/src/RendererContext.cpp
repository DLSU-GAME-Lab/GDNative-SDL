#include "RendererContext.h"
RendererContext* RendererContext::P_SHARED_INSTANCE = NULL;

void RendererContext::initialize(SDL_Renderer* pRenderer)
{
    P_SHARED_INSTANCE = new RendererContext();
    P_SHARED_INSTANCE->pRenderer = pRenderer;
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
