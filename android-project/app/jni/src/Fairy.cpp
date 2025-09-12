#include "Fairy.h"
#include "TextureManager.h"
#include "SpriteRendererSystem.h"
#include "SpriteAnimator.h"

Fairy::Fairy(float fPosX, float fPosY, float fScaleX, float fScaleY, float fRot) : AGameObject("Fairy")
{
    this->fPosX = fPosX;
    this->fPosY = fPosY;
    this->fScaleX = fScaleX;
    this->fScaleY = fScaleY;
    this->fRot = fRot;
}

Fairy::~Fairy()
{

}

void Fairy::initialize()
{
    SpriteRenderer* pSpriteRenderer = new SpriteRenderer("Fairy", fPosX, fPosY);
    SpriteRendererSystem::getInstance()->registerSpriteRenderer(pSpriteRenderer);

    auto vecSprite = TextureManager::getInstance()->getTexture("Fairy", 0, 18);
    SpriteAnimator* pSpriteAnimator = new SpriteAnimator(pSpriteRenderer, vecSprite, 12);

    this->attachComponent((AComponent*)pSpriteRenderer);
    this->attachComponent((AComponent*)pSpriteAnimator);
    pSpriteAnimator->setAnimationType(AnimationType::PINGPONG);
    pSpriteAnimator->play();
}
