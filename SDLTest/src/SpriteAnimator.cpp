#include "SpriteAnimator.h"
#include "GameObjectManager.h"

SpriteAnimator::SpriteAnimator(SpriteRenderer* pSpriteRenderer) : AAnimator("SpriteAnimator")
{
	this->pSpriteRenderer = pSpriteRenderer;
}

SpriteAnimator::SpriteAnimator(SpriteRenderer* pSpriteRenderer, std::vector<SDL_Texture*> vecTexture, Uint8 nFrameRate)
	: AAnimator("SpriteAnimator")
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

void SpriteAnimator::perform()
{
	if (mapAnims.contains(strState))
	{
		mapAnims[strState]->step(fDeltaTime);

		if (mapAnims[strState]->finished())
		{
			switch (mapAnims[strState]->getOnAnimFinished())
			{
			case OnAnimFinished::NONE:
				mapAnims[strState]->pause();
				break;

			case OnAnimFinished::STOP:
				mapAnims[strState]->stop();
				break;

			case OnAnimFinished::NEXT:
				this->setNextState();
				break;

			case OnAnimFinished::FUNC:
				this->onAnimationFinished();
				break;

			case OnAnimFinished::DELETE:
				GameObjectManager::getInstance()->deleteObject(this->pOwner);
				break;

			default:
				break;
			}
		}

		this->pSpriteRenderer->setTexture(mapAnims[strState]->getCurrentFrame());
	}
}

void SpriteAnimator::setNextState()
{
	mapAnims[strState]->stop();
	std::string nextState = mapAnims[strState]->getNextState();
	if (mapAnims.contains(nextState)) this->strState = mapAnims[strState]->getNextState();
}

void SpriteAnimator::onAnimationFinished()
{
	mapAnims[strState]->pause();
	for (auto pListener : this->vecListener)
	{
		pListener->onAnimationFinished();
	}
}

void SpriteAnimator::play(std::string strState)
{
	if (this->strState == strState ||
		this->mapAnims.empty()) return;

	if (strState.empty()) this->mapAnims[this->strState]->play();
	else
	{
		this->strState = strState;
		this->mapAnims[this->strState]->play();
	}
}

void SpriteAnimator::pause()
{
	if (this->strState.empty() ||
		this->mapAnims.empty()) return;

	this->mapAnims[this->strState]->stop();
}

void SpriteAnimator::stop()
{
	if (this->strState.empty() ||
		this->mapAnims.empty()) return;

	this->mapAnims[this->strState]->stop();
}

void SpriteAnimator::addAnimation(Animation* pAnimation)
{
	if (this->mapAnims.contains(pAnimation->getName())) return;

	this->vecAnims.push_back(pAnimation);
	this->mapAnims[pAnimation->getName()] = pAnimation;
}

void SpriteAnimator::setAnimationState(std::string strState)
{
	if (this->strState == strState || this->strState.empty()) return;
	this->strState = strState;
}

void SpriteAnimator::addListener(IAnimatorListener* pListener)
{
	this->vecListener.push_back(pListener);
}

void SpriteAnimator::removeListener(IAnimatorListener* pListener)
{
	int nIndex = -1;
	for (int i = 0; i < this->vecListener.size() && nIndex == -1; i++)
	{
		if (this->vecListener[i] == pListener)
			nIndex = i;
	}

	if (nIndex != -1) this->vecListener.erase(this->vecListener.begin() + nIndex);
}

Animation* SpriteAnimator::getCurrentAnimation()
{
	if (mapAnims.contains(strState))
		return mapAnims[strState];

	return NULL;
}
