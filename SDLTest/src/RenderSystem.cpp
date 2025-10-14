// ---------------------------------------------------------------------------
// Responsibilities: hold sprite renderers and forward draw calls.
// ---------------------------------------------------------------------------

#include "RenderSystem.h"
#include "SpriteRenderer.h"
#include "Camera.h"
#include <iostream>

void RenderSystem::updateWindowSize(SDL_Window* pWindow)
{
    // O(1): Queries window size and updates camera.
    int w, h;
    SDL_GetWindowSizeInPixels(pWindow, &w, &h);
    this->pCamera->setWindowSize(Vector2D(w, h));
}

// draw: iterate all registered sprite renderers and call draw()
// Complexity: O(S) where S = number of registered sprite renderers.
// Scaling note: as S grows, total draw calls grow linearly; each draw
// invokes GPU work (expensive constant) and may result in significant
// frame-time increases even though asymptotic complexity is linear.
void RenderSystem::draw(SDL_Renderer* pRenderer)
{
    // O(R): Iterates over all registered sprite renderers and calls draw().
    // Each sprite draw call is typically O(1), but total cost grows linearly with R.
    for (auto pSpriteRenderer : this->vecSpriteRenderers)
    {
        pSpriteRenderer->draw(pRenderer, P_SHARED_INSTANCE->pCamera);
    }
}

void RenderSystem::registerSpriteRenderer(SpriteRenderer* pSpriteRenderer)
{
    // O(1): Adds a new sprite renderer to the vector.
    this->vecSpriteRenderers.push_back(pSpriteRenderer);
}

// unregisterSpriteRenderer: searches and erases
// Complexity: O(S) for search + O(S) for erase shifting -> O(S).
// Scaling note: frequent unregister/register churn with large S increases
// overhead linearly.
void RenderSystem::unregisterSpriteRenderer(SpriteRenderer* pSpriteRenderer)
{
    // O(R): Searches for the given renderer and erases it.
    // can be optimized by using an unordered_map or free list.
    int nIndex = -1;

    for (int i = 0; i < this->vecSpriteRenderers.size() && nIndex == -1; i++)
    {
        if (this->vecSpriteRenderers[i] == pSpriteRenderer)
            nIndex = i;
    }

    if (nIndex != -1)
    {
        this->vecSpriteRenderers.erase(this->vecSpriteRenderers.begin() + nIndex);
        delete pSpriteRenderer; // Memory cleanup; can be simplified using smart pointers.
    }
}

Camera* RenderSystem::getCamera()
{
    // O(1): Getter for the camera instance.
    return this->pCamera;
}

/* * * * * * * * * * * * * * * * * * * * *
 *       SINGLETON-RELATED CONTENT       *
 * * * * * * * * * * * * * * * * * * * * */

 // ================================
 // Singleton Setup (O(1) each)
 // ================================
RenderSystem* RenderSystem::P_SHARED_INSTANCE = NULL;

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
