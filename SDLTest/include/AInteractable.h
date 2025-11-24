#pragma once
#include "BoxCollider.h"
class AInteractable : public BoxCollider
{
public:
	AInteractable(std::string strName);

	virtual void onInteract() = 0;
};

