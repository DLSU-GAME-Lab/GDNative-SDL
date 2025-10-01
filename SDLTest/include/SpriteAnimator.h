#pragma once

#include "AComponent.h"
#include "SpriteRenderer.h"
#include "EnumAnimationType.h"
#include <SDL3/SDL.h>
#include <vector>
#include <unordered_map>

class SpriteAnimator : public AComponent
{
private:
	SpriteRenderer* pSpriteRenderer;
	std::string strState;
	std::unordered_map<std::string, std::vector<SDL_Texture*>> mapAnims;

	bool bIsPlaying;
	bool bIsReverse;
	AnimationType EType;

	unsigned int nFrameIndex;
	unsigned int nFrameRate;
	float fTicks;
	float fTicksPerFrame;

public:
	SpriteAnimator(SpriteRenderer* pSpriteRenderer, unsigned int nFrameRate);
	SpriteAnimator(SpriteRenderer* pSpriteRenderer, std::vector<SDL_Texture*> vecTexture, unsigned int nFrameRate);
	~SpriteAnimator();

	virtual void perform() override;

	void stop();
	void play();

	void addAnimationState(std::string strState, std::vector<SDL_Texture*> vecTexture);
	void setAnimationState(std::string strState);
	void setAnimationType(AnimationType EType);

	std::string getCurrentAnimationState() const;
	AnimationType getAnimationType() const;
};

