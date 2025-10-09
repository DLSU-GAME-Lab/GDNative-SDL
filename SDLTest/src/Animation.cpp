#include "Animation.h"

Animation::Animation(
    std::string strName,
    std::vector<SDL_Texture*> vecFrames,
    Uint8 nFrameRate,
    AnimationType EType,
    std::string strNextState
)
{
    this->strName = strName;
    this->vecFrames = vecFrames;
    this->nFrameRate = nFrameRate;
    this->EType = EType;
    this->strNextState = strNextState;

	this->bIsPlaying = false;
	this->bIsReverse = false;
	this->fTicks = 0.0f;
	this->nFrameIndex = 0;

	if (this->vecFrames.empty()) this->pCurrentFrame = NULL;
	else this->pCurrentFrame = this->vecFrames[0];

}

void Animation::stop()
{
	this->bIsPlaying = false;
	this->nFrameIndex = 0;
	this->fTicks = 0;
}

void Animation::play()
{
	this->bIsPlaying = true;
}

void Animation::step(float fDeltaTime)
{
	this->fTicks += fDeltaTime;
	float ticksPerFrame = this->getTicksPerFrame();

	if (this->fTicks >= ticksPerFrame)
	{
		while (this->fTicks >= ticksPerFrame)
			this->fTicks -= ticksPerFrame;

		if (!this->bIsReverse) this->nFrameIndex++;
		else this->nFrameIndex--;

		switch (this->EType)
		{
		case AnimationType::ONCE:
			if (this->nFrameIndex == this->vecFrames.size())
			{
				this->stop();
			}
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
				this->bIsReverse = false;
				this->stop();
			}
			break;
		}
		this->pCurrentFrame = this->vecFrames[this->nFrameIndex];
	}
}

bool Animation::playNext()
{
	return this->EType == AnimationType::ONCE && !this->bIsPlaying && !this->strNextState.empty();
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
	return this->pCurrentFrame;
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

std::string Animation::getNextState() const
{
    return this->strNextState;
}
