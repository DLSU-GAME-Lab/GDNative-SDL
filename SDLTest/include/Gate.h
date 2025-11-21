#pragma once
#include "AGameObject.h"
class Gate : public AGameObject
{
public:
	Gate(std::string strName);

	void initialize() override;
};

