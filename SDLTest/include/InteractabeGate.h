#pragma once
#include "AInteractable.h"
class InteractabeGate : public AInteractable
{
public:
	InteractabeGate();

	void onInteract() override;
};

