#include "SpriteAnimator.h"

SpriteAnimator::SpriteAnimator(SpriteRenderer* pSpriteRenderer, std::vector<SDL_Texture*> vecTexture, unsigned int nFrameRate)
	: AComponent("SpriteAnimator", ComponentType::SCRIPT)
{
	this->pSpriteRenderer = pSpriteRenderer;
	this->vecTexture = vecTexture;
	this->nFrameRate = nFrameRate;

	this->bIsPlaying = false;
	this->bIsReverse = false;
	this->EType = AnimationType::ONCE;
	this->nFrameIndex = 0;
	this->fTicks = 0;
	this->fTicksPerFrame = nFrameRate / 60.0f;
}

SpriteAnimator::~SpriteAnimator()
{

}

void SpriteAnimator::perform()
{
	if (this->bIsPlaying)
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
				if (this->nFrameIndex == this->vecTexture.size()) this->stop();
				break;

			case AnimationType::LOOP:
				this->nFrameIndex %= this->vecTexture.size();
				break;

			case AnimationType::PINGPONG:
				if (this->nFrameIndex == this->vecTexture.size() - 1 ||
					this->nFrameIndex == 0)
				{
					this->bIsReverse = !this->bIsReverse;
				}
				break;

			default:
				break;
			}

			this->pSpriteRenderer->setTexture(this->vecTexture[this->nFrameIndex]);
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
	if (this->bIsPlaying) this->stop();
	this->bIsPlaying = true;
}

void SpriteAnimator::setAnimationType(AnimationType EType)
{
	this->EType = EType;
}

AnimationType SpriteAnimator::getAnimationType() const
{
	return this->EType;
}
