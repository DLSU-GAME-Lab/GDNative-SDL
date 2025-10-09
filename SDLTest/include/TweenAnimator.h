#pragma once
#include "AComponent.h"
#include "tweeny.h"
#include "EnumAnimationType.h"
#include "IAnimatorListener.h"

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

	std::vector<IAnimatorListener*> vecListener;

public:
	TweenAnimator();
	~TweenAnimator();
	virtual void perform() override;

	void play();
	void pause();
	void stop();

	void addListener(IAnimatorListener* pListener);
	void removeListener(IAnimatorListener* pListener);

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

