#include "AnimatedSprite.h"
#include "TextureManager.h"
#include "SpriteAnimator.h"

AnimatedSprite::AnimatedSprite(
	std::string strName,
    std::string strSpriteName,
    Vector2D fVecTranslate,
    Vector2D fVecScale,
	float fRot,
    int nFrameRate)
	: AGameObject(strName)
{
    this->strSpriteName = strSpriteName;
    this->fVecTranslate = fVecTranslate;
    this->fVecScale = fVecScale;
    this->fRot = fRot;
    this->nFrameRate = nFrameRate;
}

AnimatedSprite::~AnimatedSprite()
{

}

void AnimatedSprite::initialize()
{
    SpriteRenderer* pSpriteRenderer = new SpriteRenderer(this->strSpriteName, this->fVecTranslate.x, this->fVecTranslate.y);
    auto vecSprite = TextureManager::getInstance()->getTexture(this->strSpriteName);
    SpriteAnimator* pSpriteAnimator = new SpriteAnimator(pSpriteRenderer, vecSprite, this->nFrameRate);

    this->attachComponent((AComponent*)pSpriteRenderer);
    this->attachComponent((AComponent*)pSpriteAnimator);
    pSpriteAnimator->play();
}
