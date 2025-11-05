#pragma once
#include "AComponent.h"
class ACollectable : public AComponent
{
public:
	ACollectable(std::string strName);

	virtual void onCollect() = 0;
};

