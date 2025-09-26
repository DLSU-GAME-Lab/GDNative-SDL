#pragma once
#include "AComponent.h"
#include "ButtonInput.h"

class GemSwapper : public AComponent
{
private:
	ButtonInput* pInput;

public:
	GemSwapper(ButtonInput* pInput);
	~GemSwapper();

	void onAttach() override;
	void perform() override;
};

