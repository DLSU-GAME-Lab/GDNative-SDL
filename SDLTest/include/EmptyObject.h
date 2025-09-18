#pragma once
#include "AGameObject.h"

class EmptyObject : public AGameObject
{
public:
	EmptyObject(std::string strName) : AGameObject(strName) {};
	void initialize() override {};
};

