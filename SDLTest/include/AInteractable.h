#pragma once
#include "AComponent.h"
class AInteractable : public AComponent
{
public:
	AInteractable(std::string strName);

	virtual void onCollect() = 0;
};

