#pragma once
#include "AGameObject.h"

class Background : public AGameObject
{
private:
	std::string strImageName;
	

public:
	Background(const std::string& strName, const std::string& strImageName, Vector2D fVecScale);
	~Background();

	void initialize() override;
};

