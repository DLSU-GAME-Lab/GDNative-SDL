#include "TokenController.h"
#include "PuzzleToken.h"

TokenController::TokenController(ButtonInput* pInput) : AComponent("TokenController", ComponentType::SCRIPT)
{
	this->pInput = pInput;
	this->bPickedup = false;
	this->holdingScale = Vector2D(0.05f);
}

void TokenController::onAttach()
{
	this->initialScale = this->pOwner->getScale();
}

void TokenController::perform()
{
	if (this->pInput->getHolding())
	{
		this->bPickedup = true;
		this->pOwner->setPos(this->pInput->getMouseWorldPos());
		this->pOwner->setScale(this->holdingScale);
	}
	else if (this->bPickedup)
	{
		this->bPickedup = false;
		((PuzzleToken*)this->pOwner)->setDropped(true);
		this->pOwner->setScale(this->initialScale);
	}
}
