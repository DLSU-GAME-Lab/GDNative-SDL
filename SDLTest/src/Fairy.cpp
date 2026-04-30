#include "Fairy.h"
#include "TextureManager.h"
#include "SpriteAnimator.h"
#include "TweenAnimator.h"

Fairy::Fairy(Vector2D fVecTranslate, Vector2D fVecScale, float fRot) : AGameObject("Fairy")
{
    this->fVecTranslate = fVecTranslate;
    this->fVecScale = fVecScale;
    this->fRot = fRot;
}

void Fairy::initialize()
{
    SpriteRenderer* pSpriteRenderer = new SpriteRenderer("Fairy");
    auto vecSprite = TextureManager::getInstance()->getTexture("Fairy", 0, 18);
    SpriteAnimator* pSpriteAnimator = new SpriteAnimator(pSpriteRenderer, vecSprite, 12);
    TweenAnimator* pTweenAnimator = new TweenAnimator();
    pTweenAnimator->setLocal(true);
    pTweenAnimator->setAnimationType(AnimationType::YOYO);

    this->attachComponent(pSpriteRenderer);
    this->attachComponent(pSpriteAnimator);
    this->attachComponent(pTweenAnimator);

    Vector2D localPos = this->fVecTranslate;
    Tween2D tween = Tween2D::from(localPos.x, localPos.y)
        .to(localPos.x, localPos.y + 100)
        .during(2000)
        .via(tweeny::easing::cubicInOut);
    pTweenAnimator->setTweenPos(tween);
    pTweenAnimator->play();

}
