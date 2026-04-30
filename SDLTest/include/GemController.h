#pragma once
#include "AComponent.h"
#include "ButtonInput.h"
class GemController: public AComponent
{
private:
	ButtonInput* pInput;
	bool bPickedup;
	Vector2D initialScale;
	Vector2D holdingScale;
public:
	GemController(ButtonInput* pInput);


	void onAttach() override;
	// Inherited via AComponent
	void perform() override;

};

