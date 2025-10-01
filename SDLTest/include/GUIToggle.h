#pragma once
#include "AComponent.h"
class GUIToggle : public AComponent
{
private:
	std::string strGUIName;
public:
	GUIToggle(std::string strGUIName);
	~GUIToggle();

	void perform() override;
};

