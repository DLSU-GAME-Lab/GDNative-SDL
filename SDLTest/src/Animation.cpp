#include "Animation.h"

Animation::Animation(std::string strName, std::vector<SDL_Texture*> vecFrames, Uint8 nFrameRate)
{
    this->strName = strName;
    this->vecFrames = vecFrames;
    this->nFrameRate = nFrameRate;
}

void Animation::setFrameRate(Uint8 nFrameRate)
{
    this->nFrameRate = nFrameRate;
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
