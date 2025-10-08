#pragma once
#include "AGameObject.h"
#include "EnumGemType.h"
#include "IAnimatorListener.h"
#include "TweenAnimator.h"

class Gem : public AGameObject, public IAnimatorListener
{
private:
	GemType EType;
	TweenAnimator* pTween;

public:
	Gem(std::string strName, GemType EType);
	~Gem();

	void initialize() override;
	void onAnimationFinished() override;

	GemType getType() const;
	TweenAnimator* getTweenAnimator() const;
};

