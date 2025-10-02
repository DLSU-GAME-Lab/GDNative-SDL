#include "Fairy.h"
#include "TextureManager.h"
#include "SpriteAnimator.h"

Fairy::Fairy(Vector2D fVecTranslate, Vector2D fVecScale, float fRot) : AGameObject("Fairy")
{
    this->fVecTranslate = fVecTranslate;
    this->fVecScale = fVecScale;
    this->fRot = fRot;
}

Fairy::~Fairy()
{

}

void Fairy::initialize()
{
    SpriteRenderer* pSpriteRenderer = new SpriteRenderer("Fairy", this->fVecTranslate.x, this->fVecTranslate.y);
    auto vecSprite = TextureManager::getInstance()->getTexture("Fairy", 0, 18);
    SpriteAnimator* pSpriteAnimator = new SpriteAnimator(pSpriteRenderer, vecSprite, 12);

    this->attachComponent((AComponent*)pSpriteRenderer);
    this->attachComponent((AComponent*)pSpriteAnimator);
    pSpriteAnimator->play();
}
