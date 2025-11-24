#pragma once

#include "AAnimator.h"
#include "SpriteRenderer.h"
#include "Animation.h"
#include "IAnimatorListener.h"
#include <unordered_map>

class SpriteAnimator : public AAnimator
{
private:
	SpriteRenderer* pSpriteRenderer;
	std::string strState;
	std::vector<Animation*> vecAnims;
	std::unordered_map<std::string, Animation*> mapAnims;
	std::vector<IAnimatorListener*> vecListener;

private:
	void setNextState();
	void onAnimationFinished();

public:
	SpriteAnimator(SpriteRenderer* pSpriteRenderer);
	SpriteAnimator(SpriteRenderer* pSpriteRenderer, std::vector<SDL_Texture*> vecTexture, Uint8 nFrameRate);
	~SpriteAnimator();

	virtual void perform() override;

	void play(std::string strState = "");
	void pause();
	void stop();

	void addAnimation(Animation* pAnimation);
	void setAnimationState(std::string strState);
	void addListener(IAnimatorListener* pListener);
	void removeListener(IAnimatorListener* pListener);

	Animation* getCurrentAnimation();
};

