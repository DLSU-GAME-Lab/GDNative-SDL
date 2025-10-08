#pragma once
#include "AComponent.h"
#include "tweeny.h"
#include "EnumAnimationType.h"

typedef tweeny::tween<float> Tween;
typedef tweeny::tween<float, float> Tween2D;

class TweenAnimator : public AComponent
{
private:
	Tween2D tweenPos;
	Tween2D tweenScale;
	Tween tweenRot;

	AnimationType EType;
	bool bIsPlaying;
	bool bIsReverse;

public:
	TweenAnimator();
	~TweenAnimator();
	virtual void perform() override;

	void play();
	void pause();
	void stop();

	void setAnimationType(AnimationType EType);

	void setTweenPos(const Tween2D& tweenPos);
	void setTweenScale(const Tween2D& tweenScale);
	void setTweenRot(const Tween& tweenRot);
};

