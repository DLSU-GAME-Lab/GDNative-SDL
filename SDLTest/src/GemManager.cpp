#include "GemManager.h"
#include "GameObjectManager.h"
#include "EmptyObject.h"

void GemManager::perform()
{

}

void GemManager::startLevel()
{

}

GemManager* GemManager::P_SHARED_INSTANCE = NULL;

void GemManager::initialize()
{
    EmptyObject* pManagerObject = new EmptyObject("GUI Manager");
    P_SHARED_INSTANCE = new GemManager();
    pManagerObject->attachComponent(P_SHARED_INSTANCE);
    GameObjectManager::getInstance()->addObject(pManagerObject);
}

void GemManager::destroy()
{
    delete P_SHARED_INSTANCE->pOwner;
}

GemManager* GemManager::getInstance()
{
    return P_SHARED_INSTANCE;
}
