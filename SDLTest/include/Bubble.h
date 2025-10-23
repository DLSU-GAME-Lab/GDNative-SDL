#pragma once
#include "AGameObject.h"
#include "TweenAnimator.h"
#include "IAnimatorListener.h"

class Bubble : public AGameObject, IAnimatorListener
{
private:
	TweenAnimator* pAnimator;

public:
	Bubble(std::string strName);

	void initialize() override;
	void onAnimationFinished() override;

	void randomizeTween();
};

