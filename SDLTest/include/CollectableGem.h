#pragma once
#include "ACollectable.h"
class CollectableGem : public ACollectable
{
public:
	CollectableGem();

	void perform() override;
	void onCollect() override;
};

