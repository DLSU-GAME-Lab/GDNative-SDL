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
    if (owner)
    {
       
        if(!pOwner->getIsScreenObject() && pOwner->findComponentByName("SpriteRenderer") == NULL)
        {
            std::cout << pOwner->getName() << std::endl;
            std::cout << Rect.x << ", " << Rect.y << std::endl;
            tempRect = pCam->worldToScreenRect(this->Rect);
            SDL_SetRenderDrawColor(pRenderer, 255, 0, 0, 50);  // RGBA

        }

        else
        {
            tempRect.x = owner->getGlobalBounds().x;
            tempRect.y = owner->getGlobalBounds().y;
            tempRect.w = owner->getGlobalBounds().w;
            tempRect.h = owner->getGlobalBounds().h;
            SDL_SetRenderDrawColor(pRenderer, 0, 0, 255, 50);  // RGBA

        }
        SDL_RenderRect(pRenderer, &tempRect);
    }

}
