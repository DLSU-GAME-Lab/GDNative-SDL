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
		//Diary* pDiary = (Diary*)GameObjectManager::getInstance()->findObjectByName(this->strDiaryName);
		//if (pDiary != NULL)
		//{
		//	pDiary->otherPagesFollow();
		//	//pDiary->setEnabled(!pDiary->getEnabled());
		//	pDiary->otherPagesStopFollow();
		//}
		EventBroadcaster::getInstance()->broadcast(this->EKey);


	}
}
