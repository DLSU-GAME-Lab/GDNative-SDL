#include "GemController.h"
#include "AGameObject.h"
#include "GemInputToken.h"
#include "EventBroadcaster.h"

GemController::GemController(ButtonInput* pInput) :AComponent("GemController",ComponentType::SCRIPT)
{
	this->pInput = pInput;
	this->bPickedup = false;
	this->holdingScale = Vector2D(0.2f);
}

void GemController::onAttach()
{
	this->initialScale = this->pOwner->getScale();
}

void GemController::perform()
{
	if (this->pInput->getClicked())
	{
		this->pInput->setClicked(false);
		if (this->pInput->isLeftClick())
		{
			this->pInput->setLeftClick(false);
		}
		else if (this->pInput->isRightClick())
		{
			std::unordered_map <std::string, void*> mapParam;
			std::string strName = pOwner->getName();
			mapParam["GemName"] = static_cast<void*>(&strName);
			EventBroadcaster::getInstance()->broadcast(EventKey::GEM_INPUT_PREVIEW, mapParam);
			this->pInput->setRightClick(false);

		}
	}

	if (this->pInput->getDragging())
	{
		this->bPickedup = true;
		this->pOwner->setPos(this->pInput->getMousePos());
		this->pOwner->setScale(this->holdingScale);
	}
	else if (this->bPickedup)
	{
		this->bPickedup = false;
		((GemInputToken*)this->pOwner)->setDropped(true);
		this->pOwner->setScale(this->initialScale);
	}
}
