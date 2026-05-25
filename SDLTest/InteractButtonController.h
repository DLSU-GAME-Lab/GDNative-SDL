#pragma once
#include "AComponent.h"
#include "PlayerInput.h"
#include "SpriteRenderer.h"
class InteractButtonController:public AComponent
{
public:
	InteractButtonController();
	// Inherited via AComponent
	void perform() override;
	void onAttach() override;
private:
	PlayerInput* playerInput = nullptr;
	SpriteRenderer* pRenderer = nullptr;
	bool bIsVisible = false;
	bool bInteracted = false;
};

