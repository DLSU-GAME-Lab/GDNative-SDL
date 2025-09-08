#include "SpriteAnimator.h"
#include "EngineTime.h"

SpriteAnimator::SpriteAnimator(SpriteRenderer* pSpriteRenderer, std::vector<SDL_Texture*> vecTexture, unsigned int nFPS)
	: AComponent("SpriteAnimator", ComponentType::ANIMATOR)
{
	this->pSpriteRenderer = pSpriteRenderer;
	this->vecTexture = vecTexture;
	this->nFPS = nFPS;

	this->bPlayOnInit = true;
	this->bIsPlaying = true;
	this->bIsLooping = true;
	this->nFrameIndex = 0;
	this->nTicks = 0;
	this->nTicksPerFrame = nFPS / SDL_SECONDS_TO_NS(1);
}

SpriteAnimator::~SpriteAnimator()
{

}

void SpriteAnimator::Animate()
{
	if (this->bIsPlaying)
	{
		this->nTicks += EngineTime::getInstance()->getTrueDeltaTime();
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

void SpriteAnimator::perform()
{

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
