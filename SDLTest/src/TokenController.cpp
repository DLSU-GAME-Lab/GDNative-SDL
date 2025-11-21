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
	if (this->pInput->getClicked())
	{
		this->pInput->setClicked(false);
		if (this->pInput->isRightClick())
		{
			std::unordered_map <std::string, void*> mapParam;
			std::string strName = pOwner->getName();
			mapParam["TokenName"] = static_cast<void*>(&strName);
			EventBroadcaster::getInstance()->broadcast(EventKey::RIGHT_CLICK, mapParam);
			this->pInput->setRightClick(false);
		}
		if (this->pInput->isLeftClick())
		{
			this->pInput->setLeftClick(false);
		}
	}

	if (this->pInput->getDragging())
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
