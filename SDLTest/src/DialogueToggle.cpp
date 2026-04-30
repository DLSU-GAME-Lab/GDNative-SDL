#include "DialogueToggle.h"
#include "ButtonInput.h"
#include "EventBroadcaster.h"

DialogueToggle::DialogueToggle(unsigned int nDialogueKey):AComponent("DialogueToggle",ComponentType::SCRIPT)
{
	this->nCounter = 0;
	this->EKey = EventKey::DIALOGUE_SCREEN;
	this->nDialogueKey = nDialogueKey;
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
		mapParam["DialogueToggle"] = static_cast<void*>(&this->nDialogueKey);
		EventBroadcaster::getInstance()->broadcast(this->EKey, mapParam);
	}
}
