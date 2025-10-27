#include "GUIToggle.h"
#include "GameObjectManager.h"
#include "ButtonInput.h"

GUIToggle::GUIToggle(std::string strGUIName) : AComponent("GUIToggle", ComponentType::SCRIPT)
{
	this->strGUIName = strGUIName;
}

GUIToggle::~GUIToggle()
{

}

void GUIToggle::perform()
{
	
	ButtonInput* pInput = (ButtonInput*)pOwner->findComponentByName("ButtonInput");
	if (pInput && pInput->getClicked())
	{
		pInput->setClicked(false);
		AGameObject* pObject = GameObjectManager::getInstance()->findObjectByName(this->strGUIName);
		if (pObject != NULL)
		{
			pObject->setEnabled(!pObject->getEnabled());
		}
	}
}
