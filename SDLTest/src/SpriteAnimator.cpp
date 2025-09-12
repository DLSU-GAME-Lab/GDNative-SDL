#include "SpriteAnimator.h"
#include "EngineTime.h"

SpriteAnimator::SpriteAnimator(SpriteRenderer* pSpriteRenderer, std::vector<SDL_Texture*> vecTexture, unsigned int nFrameRate)
	: AComponent("SpriteAnimator", ComponentType::ANIMATOR)
{
	this->pSpriteRenderer = pSpriteRenderer;
	this->vecTexture = vecTexture;
	this->nFrameRate = nFrameRate;

	this->bIsPlaying = false;
	this->bIsReverse = false;
	this->EType = AnimationType::ONCE;
	this->nFrameIndex = 0;
	this->nTicks = 0;
	this->nTicksPerFrame = SDL_MS_PER_SECOND / nFrameRate;
}

SpriteAnimator::~SpriteAnimator()
{

}

void SpriteAnimator::perform()
{
	if (this->bIsPlaying)
	{
		this->nTicks += EngineTime::getInstance()->getTrueDeltaTime();
		if (this->nTicks >= this->nTicksPerFrame)
		{
			this->nTicks %= this->nTicksPerFrame;

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
	this->nTicks = 0;
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
