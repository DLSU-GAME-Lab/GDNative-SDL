#pragma once
#include "AGameObject.h"
#include "SpriteRendererSystem.h"
class Ladder : public AGameObject
{
public:
	Ladder();
	~Ladder();

	void initialize() override;
};

