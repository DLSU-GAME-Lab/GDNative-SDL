#pragma once
#include "AGameObject.h"

class GUIButton : public AGameObject
{
private:
	std::string strSpriteName;

public:
	GUIButton(std::string strName, std::string strSpriteName);
	~GUIButton();

	void initialize() override;
};

