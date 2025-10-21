#pragma once
#include "AComponent.h"
#include "ButtonInput.h"
class TokenController : public AComponent
{
private:
	ButtonInput* pInput;

public:
	TokenController(ButtonInput* pInput);
	
	void perform() override;
};

