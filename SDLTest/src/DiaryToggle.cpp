#include "DiaryToggle.h"
#include "ButtonInput.h"
#include "GameObjectManager.h"
#include "Diary.h"
DiaryToggle::DiaryToggle(EventKey EKey):AComponent("DiaryToggle", ComponentType::SCRIPT)
{
	this->EKey = EKey;
}

DiaryToggle::~DiaryToggle()
{
}

void DiaryToggle::perform()
{
	ButtonInput* pInput = (ButtonInput*)pOwner->findComponentByName("ButtonInput");
	if (pInput && pInput->getClicked())
	{
		pInput->setClicked(false);
		EventBroadcaster::getInstance()->broadcast(this->EKey);


	}
}
