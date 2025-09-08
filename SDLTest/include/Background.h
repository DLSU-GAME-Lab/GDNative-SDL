#pragma once
#include "AGameObject.h"

class Background : public AGameObject
{
private:
	std::string strImageName;
	

public:
	Background(const std::string& strName, const std::string& strImageName, float fScaleX, float fScaleY);
	~Background();

	void initialize() override;
};

