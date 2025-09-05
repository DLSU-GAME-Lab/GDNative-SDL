#pragma once
#include "AGameObject.h"
class Fairy : public AGameObject
{
public:
	Fairy();
	~Fairy();

	void initialize() override;
};

