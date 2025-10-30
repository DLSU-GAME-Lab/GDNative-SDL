#include "ColliderRenderer.h"

ColliderRenderer::ColliderRenderer(ACollider* pCollider) :ARenderer("ColliderRenderer")
{
	this->pCollider = pCollider;
}

void ColliderRenderer::perform()
{
    SDL_FRect tempRect = {};
    AGameObject* owner = this->getOwner();

    Camera* pCam = CameraManager::getInstance()->getCurrentCamera();
    tempRect.x = pCollider->getGlobalBounds().x;
    tempRect.y = pCollider->getGlobalBounds().y;
    tempRect.w = pCollider->getGlobalBounds().w;
    tempRect.h = pCollider->getGlobalBounds().h;

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
