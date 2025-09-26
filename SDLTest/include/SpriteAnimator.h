#pragma once

#include "AComponent.h"
#include "SpriteRenderer.h"
#include "EnumAnimationType.h"
#include <SDL3/SDL.h>
#include <vector>

class SpriteAnimator : public AComponent
{
private:
	SpriteRenderer* pSpriteRenderer;
	std::vector<SDL_Texture*> vecTexture;
	unsigned int nFrameRate;

	bool bIsPlaying;
	bool bIsReverse;
	AnimationType EType;

	unsigned int nFrameIndex;
	float fTicks;
	float fTicksPerFrame;

public:
	SpriteAnimator(SpriteRenderer* pSpriteRenderer, std::vector<SDL_Texture*> vecTexture, unsigned int nFrameRate);
	~SpriteAnimator();

	virtual void onAttach() override;
	virtual void perform() override;

	void stop();
	void play();

	void setAnimationType(AnimationType EType);

	AnimationType getAnimationType() const;
};

