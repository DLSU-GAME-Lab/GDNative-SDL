#include "Animation.h"
#include <cmath>

Animation::Animation(
    std::string strName,
    std::vector<SDL_Texture*> vecFrames,
    Uint8 nFrameRate,
    AnimationType EType,
	OnAnimFinished EOnFinished,
    std::string strNextState
)
{
    this->strName = strName;
    this->vecFrames = vecFrames;
    this->nFrameRate = nFrameRate;
    this->EType = EType;
	this->EOnFinished = EOnFinished;
    this->strNextState = strNextState;

	this->stop();
}

void Animation::play()
{
	this->bIsPlaying = true;
	this->bFinished = false;
}

void Animation::pause()
{
	this->bIsPlaying = false;
}

void Animation::stop()
{
	this->bIsPlaying = false;
	this->bFinished = false;
	this->bIsReverse = false;
	this->fTicks = 0.0f;
	this->nFrameIndex = 0;
}

void Animation::step(float fDeltaTime)
{
	if (!this->bIsPlaying) return;

	this->fTicks += fDeltaTime;
	float ticksPerFrame = this->getTicksPerFrame();

	if (this->fTicks >= ticksPerFrame)
	{
		this->fTicks = std::fmod(this->fTicks, ticksPerFrame);

		if (!this->bIsReverse) this->nFrameIndex++;
		else this->nFrameIndex--;

		switch (this->EType)
		{
		case AnimationType::ONCE:
			if (this->nFrameIndex == this->vecFrames.size() - 1) this->bFinished = true;
			break;

		case AnimationType::LOOP:
			this->nFrameIndex %= this->vecFrames.size();
			break;

		case AnimationType::YOYO:
			if (this->nFrameIndex == this->vecFrames.size() - 1 ||
				this->nFrameIndex == 0)
			{
				this->bIsReverse = !this->bIsReverse;
			}
			break;

		case AnimationType::YOYO_ONCE:
			if (this->nFrameIndex == this->vecFrames.size() - 1)
			{
				this->bIsReverse = true;
			}
			else if (this->nFrameIndex == 0)
			{
				this->bFinished = true;
			}
			break;
		}
	}
}

bool Animation::finished() const
{
	return this->bFinished;
}

void Animation::setFrameRate(Uint8 nFrameRate)
{
    this->nFrameRate = nFrameRate;
}

void Animation::setType(AnimationType EType)
{
    this->EType = EType;
}

void Animation::setNextState(std::string strNextState)
{
    this->strNextState = strNextState;
}

void Animation::setOnAnimFinished(OnAnimFinished EOnFinished)
{
	this->EOnFinished = EOnFinished;
}

bool Animation::isPlaying() const
{
	return this->bIsPlaying;
}

std::string Animation::getName() const
{
    return this->strName;
}

SDL_Texture* Animation::getCurrentFrame() const
{
	return this->vecFrames.empty() ? NULL : this->vecFrames[this->nFrameIndex];
}

unsigned int Animation::getFrameCount()
{
    return this->vecFrames.size();
}

float Animation::getTicksPerFrame() const
{
    return 1.0f / this->nFrameRate;
}

float Animation::getFrameRate() const
{
    return this->nFrameRate;
}

AnimationType Animation::getType() const
{
    return this->EType;
}

OnAnimFinished Animation::getOnAnimFinished() const
{
	return this->EOnFinished;
}

std::string Animation::getNextState() const
{
    return this->strNextState;
}
