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

std::string Animation::getName() const
{
    return this->strName;
}

std::vector<SDL_Texture*>& Animation::getFrames()
{
    return this->vecFrames;
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
