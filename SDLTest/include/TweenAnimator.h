#pragma once
#include "AAnimator.h"
#include "tweeny.h"
#include "EnumAnimationType.h"

typedef tweeny::tween<float> Tween;
typedef tweeny::tween<float, float> Tween2D;

class TweenAnimator : public AAnimator
{
private:
	Tween2D tweenPos;
	Tween2D tweenScale;
	Tween tweenRot;

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

private:
	template<typename... Ts>
	struct loop { bool operator()(tweeny::tween<Ts...>& t, Ts...); };

	template<typename... Ts>
	struct yoyo { bool operator()(tweeny::tween<Ts...>& t, Ts...); };

	template<typename... Ts>
	struct yoyoOnce { bool operator()(tweeny::tween<Ts...>& t, Ts...); };


};

