#include "GUIToggle.h"
#include "GameObjectManager.h"

GUIToggle::GUIToggle(std::string strGUIName) : AComponent("GUIToggle", ComponentType::SCRIPT)
{
	this->strGUIName = strGUIName;
}

GUIToggle::~GUIToggle()
{

}

void GUIToggle::onAttach()
{
	this->pInput = (ButtonInput*)pOwner->findComponentByName("ButtonInput");
}

void GUIToggle::perform()
{
	
	if (pInput && pInput->getClicked())
	{
		pInput->setClicked(false);
		AGameObject* pObject = GameObjectManager::getInstance()->findObjectByName(this->strGUIName);
		if (pObject != NULL) pObject->setEnabled(!pObject->getEnabled());
	}
}
