#include "AnimatedSprite.h"
#include "TextureManager.h"
#include "SpriteRendererSystem.h"
#include "SpriteAnimator.h"

AnimatedSprite::AnimatedSprite(
	std::string strName,
    std::string strSpriteName,
	float fPosX,
	float fPosY,
	float fScaleX,
	float fScaleY,
	float fRot,
	int nFrameStart,
	int nFrameEnd,
    int nFrameRate)
	: AGameObject(strName)
{
    this->strSpriteName = strSpriteName;
    this->fPosX = fPosX;
    this->fPosY = fPosY;
    this->fScaleX = fScaleX;
    this->fScaleY = fScaleY;
    this->fRot = fRot;
    this->nFrameStart = nFrameStart;
    this->nFrameEnd = nFrameEnd;
    this->nFrameRate = nFrameRate;
}

AnimatedSprite::~AnimatedSprite()
{

}

void AnimatedSprite::initialize()
{
    SpriteRenderer* pSpriteRenderer = new SpriteRenderer(this->strSpriteName, this->fPosX, this->fPosY);
    SpriteRendererSystem::getInstance()->registerSpriteRenderer(pSpriteRenderer);
    auto vecSprite = TextureManager::getInstance()->getTexture(this->strSpriteName, this->nFrameStart, this->nFrameEnd);
    SpriteAnimator* pSpriteAnimator = new SpriteAnimator(pSpriteRenderer, vecSprite, this->nFrameRate);

    this->attachComponent((AComponent*)pSpriteRenderer);
    this->attachComponent((AComponent*)pSpriteAnimator);
    pSpriteAnimator->setAnimationType(AnimationType::LOOP);
    pSpriteAnimator->play();
}
