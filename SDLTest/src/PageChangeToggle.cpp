#include "PageChangeToggle.h"
#include "ButtonInput.h"
#include "GameObjectManager.h"
#include "Diary.h"
PageChangeToggle::PageChangeToggle(std::string strName, bool bProgress):AComponent("PageChangeToggle",ComponentType::SCRIPT)
{
	this->strName = strName;
	this->bProgress = bProgress;
}

PageChangeToggle::~PageChangeToggle()
{
}

void PageChangeToggle::perform()
{
	ButtonInput* pInput = (ButtonInput*)pOwner->findComponentByName("ButtonInput");
	if (pInput && pInput->getClicked())
	{
		pInput->setClicked(false);
		Diary* pDiary = (Diary*)GameObjectManager::getInstance()->findObjectByName(this->strName);
		if (pDiary != NULL)
		{
			if (this->bProgress)
			{
				pDiary->progressPage();
			}
			else
			{
				pDiary->regressPage();
			}
		}


	}
}
