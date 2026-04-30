#pragma once
#include "AGameObject.h"
#include "IAnimatorListener.h"
#include "SpriteRenderer.h"
#include "SpriteAnimator.h"
#include "TweenAnimator.h"

class LobbyPlayer : public AGameObject, public IAnimatorListener
{
private:
	SpriteRenderer* pRenderer;
	SpriteAnimator* pSpriteAnimator;
	TweenAnimator* pTweenAnimator;

	AGameObject* pGUI;
	AGameObject* pLeft;
	AGameObject* pRight;
	float initialPosX;

public:
	LobbyPlayer();

	virtual void initialize() override;
	virtual void onAnimationFinished() override;

	void goLeft();
	void goRight();

	void setGUI(AGameObject* pGUI);
	void setLeftArrow(AGameObject* pLeft);
	void setRightArrow(AGameObject* pRight);
};

