#include "GUIManager.h"
#include "GameObjectManager.h"
#include "EmptyObject.h"

void GUIManager::perform()
{

}

void GUIManager::registerGUI(AGameObject* pGameObject)
{
    this->vecGUI[pGameObject->getName()] = pGameObject;
}

void GUIManager::unregisterGUI(std::string strName)
{
    this->vecGUI.erase(strName);
}

AGameObject* GUIManager::getGUI(std::string strName)
{
    return this->vecGUI[strName];
}

GUIManager* GUIManager::P_SHARED_INSTANCE = NULL;

void GUIManager::initialize()
{
    EmptyObject* pManagerObject = new EmptyObject("GUI Manager");
    P_SHARED_INSTANCE = new GUIManager();
    pManagerObject->attachComponent(P_SHARED_INSTANCE);
    GameObjectManager::getInstance()->addObject(pManagerObject);
}

void GUIManager::destroy()
{
    delete P_SHARED_INSTANCE->pOwner;
}

GUIManager* GUIManager::getInstance()
{
    return P_SHARED_INSTANCE;
}
