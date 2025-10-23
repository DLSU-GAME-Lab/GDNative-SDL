#include "SpriteAnimator.h"
SpriteAnimator::SpriteAnimator(SpriteRenderer* pSpriteRenderer)
	: AComponent("SpriteAnimator", ComponentType::SCRIPT)
{
	this->pSpriteRenderer = pSpriteRenderer;
}

SpriteAnimator::SpriteAnimator(SpriteRenderer* pSpriteRenderer, std::vector<SDL_Texture*> vecTexture, Uint8 nFrameRate)
	: AComponent("SpriteAnimator", ComponentType::SCRIPT)
{
	this->pSpriteRenderer = pSpriteRenderer;
	this->strState = "default";
	Animation* pAnimation = new Animation(strState, vecTexture, nFrameRate, AnimationType::YOYO);
	this->vecAnims.push_back(pAnimation);
	this->mapAnims[strState] = pAnimation;
	this->mapAnims[strState]->play();
}

SpriteAnimator::~SpriteAnimator()
{
	for (int i = 0; i < this->vecAnims.size(); i++)
	{
		delete this->vecAnims[i];
	}
}

void SpriteAnimator::onAttach()
{

}

void SpriteAnimator::perform()
{
	if (mapAnims.contains(strState))
	{
		mapAnims[strState]->step(fDeltaTime);

		if (mapAnims[strState]->playNext())
			this->strState = mapAnims[strState]->getNextState();

		this->pSpriteRenderer->setTexture(mapAnims[strState]->getCurrentFrame());
	}
}

void SpriteAnimator::stop()
{
	if (this->strState == strState ||
		this->strState.empty() ||
		this->mapAnims.empty()) return;

	mapAnims[strState]->stop();
}

void SpriteAnimator::play(std::string strState)
{
	if (this->strState == strState ||
		this->strState.empty() ||
		this->mapAnims.empty()) return;

	this->strState = strState;
	mapAnims[strState]->play();
}

void SpriteAnimator::addAnimation(Animation* pAnimation)
{
	this->vecAnims.push_back(pAnimation);
	this->mapAnims[pAnimation->getName()] = pAnimation;
}

void SpriteAnimator::setAnimationState(std::string strState)
{
	if (this->strState == strState || this->strState.empty()) return;
	this->strState = strState;
}

Animation* SpriteAnimator::getCurrentAnimation()
{
	if (mapAnims.contains(strState))
		return mapAnims[strState];

	return NULL;
}
