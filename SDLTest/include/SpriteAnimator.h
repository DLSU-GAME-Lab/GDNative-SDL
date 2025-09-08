#pragma once

#include "AComponent.h"
#include "SpriteRenderer.h"
#include <SDL3/SDL.h>
#include <vector>

class SpriteAnimator : public AComponent
{
private:
	SpriteRenderer* pSpriteRenderer;
	std::vector<SDL_Texture*> vecTexture;
	unsigned int nFrameRate;

	bool bIsPlaying;
	bool bIsLooping;
	unsigned int nFrameIndex;
	unsigned int nTicks;
	unsigned int nTicksPerFrame;

public:
	SpriteAnimator(SpriteRenderer* pSpriteRenderer, std::vector<SDL_Texture*> vecTexture, unsigned int nFrameRate);
	~SpriteAnimator();

	virtual void perform() override;

	void stop();
	void play();

	void setIsLooping(bool bIsLooping);

	bool getIsLooping() const;
};

