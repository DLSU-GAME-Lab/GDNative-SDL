#include "SpriteAnimator.h"

SpriteAnimator::SpriteAnimator(SpriteRenderer* pSpriteRenderer, unsigned int nFrameRate)
	: AComponent("SpriteAnimator", ComponentType::SCRIPT)
{
	this->pSpriteRenderer = pSpriteRenderer;
	this->strState = "default";

	this->bIsPlaying = false;
	this->bIsReverse = false;
	this->EType = AnimationType::ONCE;

	this->nFrameIndex = 0;
	this->nFrameRate = nFrameRate;
	this->fTicks = 0;
	this->fTicksPerFrame = nFrameRate / 60.0f;
}

SpriteAnimator::SpriteAnimator(SpriteRenderer* pSpriteRenderer, std::vector<SDL_Texture*> vecTexture, unsigned int nFrameRate)
	: AComponent("SpriteAnimator", ComponentType::SCRIPT)
{
	this->pSpriteRenderer = pSpriteRenderer;
	this->strState = "default";
	this->mapAnims[this->strState] = vecTexture;

	this->bIsPlaying = false;
	this->bIsReverse = false;
	this->EType = AnimationType::ONCE;

	this->nFrameIndex = 0;
	this->nFrameRate = nFrameRate;
	this->fTicks = 0;
	this->fTicksPerFrame = nFrameRate / 60.0f;
}

SpriteAnimator::~SpriteAnimator()
{

}

void SpriteAnimator::perform()
{
	if (!this->mapAnims.empty() && this->bIsPlaying)
	{
		this->fTicks += fDeltaTime;
		if (this->fTicks >= this->fTicksPerFrame)
		{
			this->fTicks -= this->fTicksPerFrame;
			//std::cout << this->fTicks << " " << this->fDeltaTime << " " << this->fTicksPerFrame << "\n";

			if (!this->bIsReverse) this->nFrameIndex++;
			else this->nFrameIndex--;
			
			switch (this->EType)
			{
			case AnimationType::ONCE:
				if (this->nFrameIndex == mapAnims[strState].size()) this->stop();
				break;

			case AnimationType::LOOP:
				this->nFrameIndex %= mapAnims[strState].size();
				break;

			case AnimationType::PINGPONG:
				if (this->nFrameIndex == mapAnims[strState].size() - 1 ||
					this->nFrameIndex == 0)
				{
					this->bIsReverse = !this->bIsReverse;
				}
				break;

			default:
				break;
			}

			this->pSpriteRenderer->setTexture(mapAnims[strState][this->nFrameIndex]);
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

void SpriteAnimator::addAnimationState(std::string strState, std::vector<SDL_Texture*> vecTexture)
{
	this->mapAnims[strState] = vecTexture;
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
}

void SpriteAnimator::setAnimationType(AnimationType EType)
{
	this->EType = EType;
}

std::string SpriteAnimator::getCurrentAnimationState() const
{
	return this->strState;
}

AnimationType SpriteAnimator::getAnimationType() const
{
	return this->EType;
}
