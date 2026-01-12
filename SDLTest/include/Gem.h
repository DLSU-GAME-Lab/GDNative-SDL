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

	Uint8 r;
	Uint8 c;

public:
	Gem(std::string strName, GemType EType);
	~Gem();

	void initialize() override;
	void onAnimationFinished() override;
	void setActive(bool bActive);

	GemType getType() const;
	void setType(GemType EType);
	Uint8 getRow() const;
	Uint8 getCol() const;
	void setGridPosition(Uint8 r, Uint8 c);
	TweenAnimator* getTweenAnimator() const;
};

