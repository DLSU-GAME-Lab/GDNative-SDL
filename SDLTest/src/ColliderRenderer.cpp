#include "ColliderRenderer.h"

ColliderRenderer::ColliderRenderer(ACollider* pCollider) :ARenderer("ColliderRenderer")
{
	this->pCollider = pCollider;
}

void ColliderRenderer::perform()
{
    Camera* pCam = CameraManager::getInstance()->getCurrentCamera();
    SDL_FRect tempRect = pCollider->getGlobalBounds();
    AGameObject* owner = this->getOwner();

    if (!pOwner->getIsScreenObject()) tempRect = pCam->worldToScreenRect(tempRect);
    if (this->inCameraView(tempRect))
    {
        SDL_SetRenderDrawColor(pRenderer, 255, 0, 0, 50);  // RGBA
        SDL_RenderRect(pRenderer, &tempRect);
    }
}

void ColliderRenderer::drawWidget()
{

}
