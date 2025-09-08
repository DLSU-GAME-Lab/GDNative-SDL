#include "SpriteAnimator.h"
#include "EngineTime.h"

SpriteAnimator::SpriteAnimator(SpriteRenderer* pSpriteRenderer, std::vector<SDL_Texture*> vecTexture, unsigned int nFrameRate)
	: AComponent("SpriteAnimator", ComponentType::ANIMATOR)
{
	this->pSpriteRenderer = pSpriteRenderer;
	this->vecTexture = vecTexture;
	this->nFrameRate = nFrameRate;

	this->bIsPlaying = false;
	this->bIsLooping = true;
	this->nFrameIndex = 0;
	this->nTicks = 0;
	this->nTicksPerFrame = SDL_SECONDS_TO_NS(1) / nFrameRate;
}

SpriteAnimator::~SpriteAnimator()
{

}

void SpriteAnimator::perform()
{
	if (this->bIsPlaying)
	{
		this->nTicks += EngineTime::getInstance()->getTrueDeltaTime();
		SDL_Delay(1);
		if (this->nTicks >= this->nTicksPerFrame)
		{
			this->nTicks %= this->nTicksPerFrame;
			this->nFrameIndex++;

			if (this->nFrameIndex == this->vecTexture.size())
			{
				if (this->bIsLooping) this->nFrameIndex = 0;
				else this->stop();
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

void SpriteAnimator::setIsLooping(bool bIsLooping)
{
	this->bIsLooping = bIsLooping;
}

bool SpriteAnimator::getIsLooping() const
{
	return this->bIsLooping;
}
