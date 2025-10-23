#include "ColliderRenderer.h"

ColliderRenderer::ColliderRenderer(SDL_FRect Rect) :ARenderer("ColliderRenderer")
{
	this->Rect = Rect;

}

void ColliderRenderer::perform()
{
    SDL_FRect tempRect;
    AGameObject* owner = this->getOwner();

    Camera* pCam = CameraManager::getInstance()->getCurrentCamera();
    ACollider* pCollider = (ACollider*)owner->findComponentByName(owner->getName() + " Collider");
    if (owner)
    {
        tempRect.x = pCollider->getGlobalBounds().x;
        tempRect.y = pCollider->getGlobalBounds().y;
        tempRect.w = pCollider->getGlobalBounds().w;
        tempRect.h = pCollider->getGlobalBounds().h;

        if(!pOwner->getIsScreenObject() && !pOwner->componentExists("SpriteRenderer"))
        {
            SDL_SetRenderDrawColor(pRenderer, 255, 0, 0, 50);  // RGBA

        }

        else if(!pOwner->getIsScreenObject() && pOwner->componentExists("SpriteRenderer"))
        {

            SDL_SetRenderDrawColor(pRenderer, 0, 0, 255, 50);  // RGBA

        }
        SDL_RenderRect(pRenderer, &tempRect);
    }

}

SDL_FRect ColliderRenderer::getRect()
{
    return this->Rect;
}
