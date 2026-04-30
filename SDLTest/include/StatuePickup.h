#pragma once
#include "AInteractable.h"
class StatuePickup :public AInteractable
{
public:
	StatuePickup();

	void perform() override;
	void onInteract() override;
};

