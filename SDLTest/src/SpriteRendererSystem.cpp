#include "SpriteRendererSystem.h"
#include "SpriteRenderer.h"
#include <iostream>

/* * * * * * * * * * * * * * * * * * * * *
 *       SINGLETON-RELATED CONTENT       *
 * * * * * * * * * * * * * * * * * * * * */
SpriteRendererSystem* SpriteRendererSystem::P_SHARED_INSTANCE = NULL;

void SpriteRendererSystem::draw()
{
    for (auto pSpriteRenderer : this->vecSpriteRenderers)
        pSpriteRenderer->draw();
}

void SpriteRendererSystem::registerSpriteRenderer(SpriteRenderer* pSpriteRenderer)
{
    this->vecSpriteRenderers.push_back(pSpriteRenderer);
}

void SpriteRendererSystem::unregisterSpriteRenderer(SpriteRenderer * pSpriteRenderer)
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

void SpriteRendererSystem::initialize()
{
    P_SHARED_INSTANCE = new SpriteRendererSystem();
}

void SpriteRendererSystem::destroy()
{
    delete P_SHARED_INSTANCE;
}

SpriteRendererSystem* SpriteRendererSystem::getInstance()
{
    return P_SHARED_INSTANCE;
}
/* * * * * * * * * * * * * * * * * * * * */
