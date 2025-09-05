#pragma once
#include "AGameObject.h"
#include "SpriteRendererSystem.h"

class Lamps : public AGameObject
{
public:
	Lamps();
	~Lamps();

	void initialize() override;
};

