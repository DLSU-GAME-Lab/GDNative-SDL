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

public:
	SpriteAnimator(SpriteRenderer* pSpriteRenderer);
	SpriteAnimator(SpriteRenderer* pSpriteRenderer, std::vector<SDL_Texture*> vecTexture, Uint8 nFrameRate);
	~SpriteAnimator();

	virtual void perform() override;

	void stop();
	void reset();
	void play(std::string strState = "");

	void addAnimation(Animation* pAnimation);
	void setAnimationState(std::string strState);

	Animation* getCurrentAnimation();
};

