#include "DialogueToggle.h"
#include"ButtonInput.h"
#include "EventBroadcaster.h"
#include "AGameObject.h"
DialogueToggle::DialogueToggle(EventKey EKey):AComponent("DialogueToggle",ComponentType::SCRIPT)
{
	this->nCounter = 0;
	this->EKey = EKey;
}

DialogueToggle::~DialogueToggle()
{
}

void DialogueToggle::perform()
{
	ButtonInput* pInput = (ButtonInput*)pOwner->findComponentByName("ButtonInput");
	if (pInput && pInput->getClicked())
	{
		pInput->setClicked(false);
		std::unordered_map <std::string, void*> mapParam;
		std::string strName = this->getName();
		mapParam["Sender"] = static_cast<void*>(&strName);
		EventBroadcaster::getInstance()->broadcast(this->EKey, mapParam);
	}
}
