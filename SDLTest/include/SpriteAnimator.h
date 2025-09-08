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
	unsigned int nFPS;

	bool bPlayOnInit = true;
	bool bIsPlaying = true;
	bool bIsLooping = true;
	unsigned int nFrameIndex = 0;
	unsigned int nTicks = 0;
	unsigned int nTicksPerFrame = 0;

public:
	SpriteAnimator(SpriteRenderer* pSpriteRenderer, std::vector<SDL_Texture*> vecTexture, unsigned int nFPS);
	~SpriteAnimator();

	void Animate();
	virtual void perform() override;

	void stop();
	void play();

	void setFrames(std::vector<SDL_Texture*> vecTexture);
	std::vector<SDL_Texture*> getFrames() const;
};

