#pragma once
#include "AComponent.h"
class GUIToggle : public AComponent
{
private:
	std::string strGUIName;
public:
	GUIToggle(std::string strGUIName, std::string strNumber = "0");
	~GUIToggle();

	void perform() override;
};

