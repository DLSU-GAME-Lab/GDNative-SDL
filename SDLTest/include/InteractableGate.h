#pragma once
#include "AInteractable.h"
class InteractableGate : public AInteractable
{
public:
	InteractableGate();

	void onInteract() override;
};

