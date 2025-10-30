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

	AnimationType EType;
	Uint8 nFrameRate;
	std::string strNextState;

	bool bIsPlaying;
	bool bIsReverse;
	unsigned int nFrameIndex;
	float fTicks;

public:
	Animation(
		std::string strName,
		std::vector<SDL_Texture*> vecFrames,
		Uint8 nFrameRate,
		AnimationType EType,
		std::string strNextState = ""
	);

	void stop();
	void play();
	void reset();
	void step(float fDeltaTime);
	bool playNext();

	void setFrameRate(Uint8 nFrameRate);
	void setType(AnimationType EType);
	void setNextState(std::string strNextState);

	bool isPlaying() const;
	std::string getName() const;
	SDL_Texture* getCurrentFrame() const;
	unsigned int getFrameCount();
	float getTicksPerFrame() const;
	float getFrameRate() const;
	AnimationType getType() const;
	std::string getNextState() const;
};

