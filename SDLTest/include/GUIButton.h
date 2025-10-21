#pragma once
#include "AGameObject.h"

class GUIButton : public AGameObject
{
private:
	std::string strSpriteName;
	bool bFlipX;
public:
	GUIButton(std::string strName, std::string strSpriteName, bool bFlipX = false);

	void initialize() override;
};

