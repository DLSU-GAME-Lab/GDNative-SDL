#pragma once
#include "AComponent.h"
#include "ButtonInput.h"

class GUIToggle : public AComponent
{
private:
	std::string strGUIName;
	ButtonInput* pInput;

public:
	GUIToggle(std::string strGUIName);
	~GUIToggle();

	void onAttach() override;
	void perform() override;
};
