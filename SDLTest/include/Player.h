#pragma once
#include "AGameObject.h"

class Player : public AGameObject
{
public:
	Player();
	~Player();

	void initialize() override;
};

