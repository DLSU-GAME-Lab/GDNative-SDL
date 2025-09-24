#include "GameObjectManager.h"
#include <iostream>

void GameObjectManager::processInput(SDL_Event* eEvent)
{
    for(AGameObject* pGameObject : this->vecGameObject)
    {
        if(pGameObject->getEnabled())
            pGameObject->processInput(eEvent);
    }
}

void GameObjectManager::update(float fDeltaTime)
{
    for(AGameObject* pGameObject : this->vecGameObject)
    {
        if(pGameObject->getEnabled())
            pGameObject->update(fDeltaTime);
    }
}

void GameObjectManager::draw(SDL_Renderer* pRenderer)
{
    for(AGameObject* pGameObject : this->vecGameObject)
    {
        if(pGameObject->getEnabled())
            pGameObject->draw(pRenderer);
    }
}

void GameObjectManager::addObject(AGameObject* pGameObject)
{
    this->vecGameObject.push_back(pGameObject);
    this->mapGameObject[pGameObject->getName()] = pGameObject;
    pGameObject->initialize();
}

void GameObjectManager::deleteObject(AGameObject* pGameObject)
{
    std::string strName = pGameObject->getName();
    int nIndex = -1;

    for(int i = 0; i < this->vecGameObject.size() && nIndex == -1; i++)
    {
        if(this->vecGameObject[i] == pGameObject)
            nIndex = i;
    }

    if(nIndex != -1) {
        this->mapGameObject.erase(this->vecGameObject[nIndex]->getName());
        this->vecGameObject.erase(this->vecGameObject.begin() + nIndex);
        delete pGameObject;
    }
}

void GameObjectManager::deleteObjectByName(std::string strName)
{
    AGameObject* pGameObject = this->findObjectByName(strName);
    if(pGameObject != NULL)
        this->deleteObject(pGameObject);
}

void GameObjectManager::deleteAllObjects()
{
    std::vector<AGameObject*> vecGameObject = this->vecGameObject;

    for(AGameObject* pGameObject : vecGameObject)
        this->deleteObject(pGameObject);

    this->vecGameObject.clear();
    this->mapGameObject.clear();
}

AGameObject* GameObjectManager::findObjectByName(std::string strName)
{
    if(this->mapGameObject[strName] != NULL)
        return this->mapGameObject[strName];

    else {
        std::cout << "[ERROR] : Object [" << strName << "] NOT found." << std::endl;
        return NULL;
    }
}

std::vector<AGameObject*>& GameObjectManager::getAllObjects()
{
    return this->vecGameObject;
}

/* * * * * * * * * * * * * * * * * * * * *
 *       SINGLETON-RELATED CONTENT       *
 * * * * * * * * * * * * * * * * * * * * */
GameObjectManager* GameObjectManager::P_SHARED_INSTANCE = NULL;

void GameObjectManager::initialize()
{
    P_SHARED_INSTANCE = new GameObjectManager();
}

void GameObjectManager::destroy()
{
    delete P_SHARED_INSTANCE;
}

GameObjectManager* GameObjectManager::getInstance()
{
    return P_SHARED_INSTANCE;
}
/* * * * * * * * * * * * * * * * * * * * */
