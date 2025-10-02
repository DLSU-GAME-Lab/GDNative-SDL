#pragma once

#include "EnumAnimationType.h"
#include <SDL3/SDL.h>
#include <string>
#include <vector>

class Animation
{
private:
	std::string strName;
	std::vector<SDL_Texture*> vecFrames;
	Uint8 nFrameRate;

	AnimationType EType;
	std::string strNextState;

public:
	Animation(
		std::string strName,
		std::vector<SDL_Texture*> vecFrames,
		Uint8 nFrameRate,
		AnimationType EType,
		std::string strNextState = ""
	);

	void setFrameRate(Uint8 nFrameRate);
	void setType(AnimationType EType);
	void setNextState(std::string strNextState);

	std::string getName() const;
	std::vector<SDL_Texture*>& getFrames();
	unsigned int getFrameCount();
	float getTicksPerFrame() const;
	float getFrameRate() const;
	AnimationType getType() const;
	std::string getNextState() const;
};

