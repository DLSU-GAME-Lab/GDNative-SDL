#include "GUIToggle.h"
#include "GameObjectManager.h"

GUIToggle::GUIToggle(std::string strGUIName) : AComponent("GUIToggle", ComponentType::SCRIPT)
{
	this->strGUIName = strGUIName;
}

GUIToggle::~GUIToggle()
{

}

void GUIToggle::perform()
{
	AGameObject* pObject = GameObjectManager::getInstance()->findObjectByName(this->strGUIName);
	if (pObject != NULL) pObject->setEnabled(!pObject->getEnabled());
}
