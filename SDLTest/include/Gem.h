#pragma once
#include "AGameObject.h"
#include "EnumGemType.h"
#include "TweenAnimator.h"
#include "IAnimatorListener.h"

class Gem : public AGameObject, public IAnimatorListener
{
private:
	GemType EType;
	TweenAnimator* pTween;
	bool bActive;

public:
	Gem(std::string strName, GemType EType);
	~Gem();

	void initialize() override;
	void onAnimationFinished() override;
	void setActive(bool bActive);

	GemType getType() const;
	void setType(GemType EType);
	TweenAnimator* getTweenAnimator() const;
};

