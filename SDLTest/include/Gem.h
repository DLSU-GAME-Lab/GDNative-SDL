#pragma once
#include "AGameObject.h"

class Gem : public AGameObject
{
public:
	Gem(std::string strName);

	void initialize() override;
};

