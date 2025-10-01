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
		std::cout << pOwner->getName() << std::endl;
		pInput->setClicked(false);
		AGameObject* pObject = GameObjectManager::getInstance()->findObjectByName(this->strGUIName);
		if (pObject != NULL)
		{
			//std::cout << pObject->getName()<<std::endl;
			pObject->setEnabled(!pObject->getEnabled());
		}
		else
		{
			std::cout << pObject->getName() << " Not Found" << std::endl;

		}
	}
}
