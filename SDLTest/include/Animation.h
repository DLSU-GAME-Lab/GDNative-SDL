#pragma once
#include <SDL3/SDL.h>
#include <string>
#include <vector>

class Animation
{
private:
	std::string strName;
	std::vector<SDL_Texture*> vecFrames;
	Uint8 nFrameRate;

public:
	Animation(std::string strName, std::vector<SDL_Texture*> vecFrames, Uint8 nFrameRate);

	void setFrameRate(Uint8 nFrameRate);

	std::vector<SDL_Texture*>& getFrames();
	unsigned int getFrameCount();
	float getTicksPerFrame() const;
	float getFrameRate() const;
};

