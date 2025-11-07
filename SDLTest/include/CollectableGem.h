#pragma once
#include "AInteractable.h"
class CollectableGem : public AInteractable
{
public:
	CollectableGem();

	void perform() override;
	void onCollect() override;
};

