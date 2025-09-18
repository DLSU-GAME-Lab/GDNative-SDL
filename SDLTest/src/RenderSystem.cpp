#include "RenderSystem.h"
#include "SpriteRenderer.h"
#include "Camera.h"
#include <iostream>

/* * * * * * * * * * * * * * * * * * * * *
 *       SINGLETON-RELATED CONTENT       *
 * * * * * * * * * * * * * * * * * * * * */
RenderSystem* RenderSystem::P_SHARED_INSTANCE = NULL;

void RenderSystem::updateWindowSize(SDL_Window* pWindow)
{
    int w, h;
    SDL_GetWindowSizeInPixels(pWindow, &w, &h);
    this->pCamera->setWindowSize(Vector2D(w, h));
}

void RenderSystem::draw(SDL_Renderer* pRenderer)
{
    for (auto pSpriteRenderer : this->vecSpriteRenderers)
    {
        pSpriteRenderer->draw(pRenderer, P_SHARED_INSTANCE->pCamera);
    }
}

void RenderSystem::registerSpriteRenderer(SpriteRenderer* pSpriteRenderer)
{
    this->vecSpriteRenderers.push_back(pSpriteRenderer);
}

void RenderSystem::unregisterSpriteRenderer(SpriteRenderer * pSpriteRenderer)
{
    int nIndex = -1;

    for (int i = 0; i < this->vecSpriteRenderers.size() && nIndex == -1; i++)
    {
        if (this->vecSpriteRenderers[i] == pSpriteRenderer)
            nIndex = i;
    }

    if (nIndex != -1)
    {
        this->vecSpriteRenderers.erase(this->vecSpriteRenderers.begin() + nIndex);
        delete pSpriteRenderer;
    }
}

Camera* RenderSystem::getCamera()
{
    return this->pCamera;
}

void RenderSystem::initialize()
{
    P_SHARED_INSTANCE = new RenderSystem();

    P_SHARED_INSTANCE->pCamera = new Camera();
}

void RenderSystem::destroy()
{
    delete P_SHARED_INSTANCE;
}

RenderSystem* RenderSystem::getInstance()
{
    return P_SHARED_INSTANCE;
}
/* * * * * * * * * * * * * * * * * * * * */
