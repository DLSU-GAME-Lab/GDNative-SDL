#pragma once

#include "AComponent.h"
#include "EnumAnimationType.h"
#include "IAnimatorListener.h"
#include <vector>

class AAnimator : public AComponent
{
protected:
	AnimationType EType;
	bool bIsPlaying;
	bool bIsReverse;

	std::vector<IAnimatorListener*> vecListener;

public:
	AAnimator(std::string strName);

	virtual void perform() = 0;

	void addListener(IAnimatorListener* pListener);
	void removeListener(IAnimatorListener* pListener);

};

