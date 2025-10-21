#include "TokenController.h"
#include "AGameObject.h"

TokenController::TokenController(ButtonInput* pInput) : AComponent("TokenController", ComponentType::SCRIPT)
{
	this->pInput = pInput;
}

void TokenController::perform()
{
	if (this->pInput->getHolding()) this->pOwner->setPos(this->pInput->getMouseWorldPos());
}
