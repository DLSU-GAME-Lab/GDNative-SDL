#pragma once
#include "AGameObject.h"
class Statue: public AGameObject
{
public:
	Statue(std::string strName);

	void initialize() override;
};

