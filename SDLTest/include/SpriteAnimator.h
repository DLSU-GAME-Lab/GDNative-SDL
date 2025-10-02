#pragma once

#include "AComponent.h"
#include "SpriteRenderer.h"
#include "Animation.h"
#include <unordered_map>

class SpriteAnimator : public AComponent
{
private:
	SpriteRenderer* pSpriteRenderer;
	std::string strState;
	std::vector<Animation*> vecAnims;
	std::unordered_map<std::string, Animation*> mapAnims;

	bool bIsPlaying;
	bool bIsReverse;

	unsigned int nFrameIndex;
	float fTicks;

public:
	SpriteAnimator(SpriteRenderer* pSpriteRenderer);
	SpriteAnimator(SpriteRenderer* pSpriteRenderer, std::vector<SDL_Texture*> vecTexture, Uint8 nFrameRate);
	~SpriteAnimator();

	virtual void perform() override;

	void stop();
	void play();

	void addAnimationState(Animation* pAnimation);
	void setAnimationState(std::string strState);

	std::string getCurrentAnimationState() const;
};

