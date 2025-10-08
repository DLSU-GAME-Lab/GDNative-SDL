#include "Librarian.h"
#include "TextureManager.h"
#include "SpriteAnimator.h"

Librarian::Librarian(Vector2D fVecTranslate, Vector2D fVecScale, float fRot):AGameObject("Librarian")
{
    this->fVecTranslate = fVecTranslate;
    this->fVecScale = fVecScale;
}

Librarian::~Librarian()
{

}

void Librarian::initialize()
{
    //1000, 350
    SpriteRenderer* pSpriteRenderer = new SpriteRenderer("Librarian", this->fVecTranslate.x, this->fVecTranslate.y);
    auto vecSprite = TextureManager::getInstance()->getTexture("Librarian", 0, 6);
    SpriteAnimator* pSpriteAnimator = new SpriteAnimator(pSpriteRenderer, vecSprite, 8);

    this->attachComponent((AComponent*)pSpriteRenderer);
    this->attachComponent((AComponent*)pSpriteAnimator);

    pSpriteRenderer->setFlipX(true);
}
