#include "Librarian.h"
#include "TextureManager.h"
#include "SpriteRendererSystem.h"
#include "SpriteAnimator.h"

Librarian::Librarian(float fPosX, float fPosY, float fScaleX, float fScaleY, float fRot) : AGameObject("Librarian")
{
    this->fPosX = fPosX;
    this->fPosY = fPosY;
    this->fScaleX = fScaleX;
    this->fScaleY = fScaleY;
    this->fRot = fRot;
}

Librarian::~Librarian()
{

}

void Librarian::initialize()
{
    //1000, 350
    SpriteRenderer* pSpriteRenderer = new SpriteRenderer("Librarian", fPosX, fPosY);
    SpriteRendererSystem::getInstance()->registerSpriteRenderer(pSpriteRenderer);

    auto vecSprite = TextureManager::getInstance()->getTexture("Librarian", 0, 6);
    SpriteAnimator* pSpriteAnimator = new SpriteAnimator(pSpriteRenderer, vecSprite, 12);

    this->attachComponent((AComponent*)pSpriteRenderer);
    this->attachComponent((AComponent*)pSpriteAnimator);

    pSpriteRenderer->setFlipX(true);
    pSpriteAnimator->play();
}
