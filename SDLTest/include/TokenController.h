#pragma once
#include "AComponent.h"
#include "ButtonInput.h"
#include "EventBroadcaster.h"
class TokenController : public AComponent
{
private:
	ButtonInput* pInput;
	bool bPickedup;
	Vector2D initialScale;
	Vector2D holdingScale;

public:
	TokenController(ButtonInput* pInput);
	
	void onAttach() override;
	void perform() override;

private:
	bool isMinDistance(Vector2D pos);
};

