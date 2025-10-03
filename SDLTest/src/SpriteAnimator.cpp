#include "SpriteAnimator.h"

SpriteAnimator::SpriteAnimator(SpriteRenderer* pSpriteRenderer)
	: AComponent("SpriteAnimator", ComponentType::SCRIPT)
{
	this->pSpriteRenderer = pSpriteRenderer;
	this->strState = "default";

	this->bIsPlaying = false;
	this->bIsReverse = false;

	this->nFrameIndex = 0;
	this->fTicks = 0;
}

SpriteAnimator::SpriteAnimator(SpriteRenderer* pSpriteRenderer, std::vector<SDL_Texture*> vecTexture, Uint8 nFrameRate)
	: AComponent("SpriteAnimator", ComponentType::SCRIPT)
{
	this->pSpriteRenderer = pSpriteRenderer;
	this->strState = "default";
	Animation* pAnimation = new Animation(strState, vecTexture, nFrameRate, AnimationType::PINGPONG);
	this->vecAnims.push_back(pAnimation);
	this->mapAnims[strState] = pAnimation;

	this->bIsPlaying = false;
	this->bIsReverse = false;

	this->nFrameIndex = 0;
	this->fTicks = 0;
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
	if (!this->mapAnims.empty() && this->bIsPlaying)
	{
		this->fTicks += fDeltaTime;
		if (this->fTicks >= mapAnims[strState]->getTicksPerFrame())
		{
			while (this->fTicks >= mapAnims[strState]->getTicksPerFrame())
				this->fTicks -= mapAnims[strState]->getTicksPerFrame();

			if (!this->bIsReverse) this->nFrameIndex++;
			else this->nFrameIndex--;
			
			switch (mapAnims[strState]->getType())
			{
			case AnimationType::ONCE:
				if (this->nFrameIndex == mapAnims[strState]->getFrameCount())
				{
					if (mapAnims[strState]->getNextState().empty()) this->stop();
					else this->setAnimationState(mapAnims[strState]->getNextState());
				}
				break;

			case AnimationType::LOOP:
				this->nFrameIndex %= mapAnims[strState]->getFrameCount();
				break;

			case AnimationType::PINGPONG:
				if (this->nFrameIndex == mapAnims[strState]->getFrameCount() - 1 ||
					this->nFrameIndex == 0)
				{
					this->bIsReverse = !this->bIsReverse;
				}
				break;

			default:
				break;
			}

			this->pSpriteRenderer->setTexture(mapAnims[strState]->getFrames()[this->nFrameIndex]);
		}
	}
}

void SpriteAnimator::stop()
{
	this->bIsPlaying = false;
	this->nFrameIndex = 0;
	this->fTicks = 0;
}

void SpriteAnimator::play()
{
	this->bIsPlaying = true;
}

void SpriteAnimator::addAnimationState(Animation* pAnimation)
{
	this->vecAnims.push_back(pAnimation);
	this->mapAnims[pAnimation->getName()] = pAnimation;
}

void SpriteAnimator::setAnimationState(std::string strState)
{
	if (this->strState == strState) return;

	this->strState = strState;
	if (this->bIsPlaying)
	{
		this->stop();
		this->play();
	}
	else this->play();
}

std::string SpriteAnimator::getCurrentAnimationState() const
{
	return this->strState;
}
