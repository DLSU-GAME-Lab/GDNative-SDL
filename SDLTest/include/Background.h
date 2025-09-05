#pragma once
#include "AGameObject.h"

class Background : public AGameObject
{
private:
	std::string strImageName;

public:
	Background(std::string strName, std::string strImageName);
	~Background();
};

