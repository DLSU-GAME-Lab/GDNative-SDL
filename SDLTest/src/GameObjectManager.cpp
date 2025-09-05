#include "GameObjectManager.h"
#include <iostream>

void GameObjectManager::processInput(SDL_Event eEvent)
{
    for(GameObject* pGameObject : this->vecGameObject)
    {
        if(pGameObject->isEnabled())
            pGameObject->processInput(eEvent);
    }
}

void GameObjectManager::update() {
    for(GameObject* pGameObject : this->vecGameObject)
    {
        if(pGameObject->isEnabled()/* && !GameStateManager::getInstance()->isPaused()*/)
            pGameObject->update();
    }
}

void GameObjectManager::draw(SDL_Window* pWindow)
{
    for(GameObject* pGameObject : this->vecGameObject)
    {
        if(pGameObject->isEnabled())
            pGameObject->draw(pWindow);
    }
}

void GameObjectManager::addObject(GameObject* pGameObject)
{
    this->vecGameObject.push_back(pGameObject);
    this->mapGameObject[pGameObject->getName()] = pGameObject;
    pGameObject->initialize();
}

void GameObjectManager::deleteObject(GameObject* pGameObject)
{
    std::string strName = pGameObject->getName();
    int nIndex = -1;

    for(int i = 0; i < this->vecGameObject.size() && nIndex == -1; i++)
    {
        if(this->vecGameObject[i] == pGameObject)
            nIndex = i;
    }

    if(nIndex != -1) {
        this->vecGameObject.erase(this->vecGameObject.begin() + nIndex);
        this->mapGameObject.erase(this->vecGameObject[nIndex]->getName());
        delete pGameObject;
    }
}

void GameObjectManager::deleteObjectByName(std::string strName)
{
    GameObject* pGameObject = this->findObjectByName(strName);
    if(pGameObject != NULL)
        this->deleteObject(pGameObject);
}

void GameObjectManager::deleteAllObjects()
{
    std::vector<GameObject*> vecGameObject = this->vecGameObject;

    for(GameObject* pGameObject : vecGameObject)
        this->deleteObject(pGameObject);

    this->vecGameObject.clear();
    this->mapGameObject.clear();
}

GameObject* GameObjectManager::findObjectByName(std::string strName)
{
    if(this->mapGameObject[strName] != NULL)
        return this->mapGameObject[strName];

    else {
        std::cout << "[ERROR] : Object [" << strName << "] NOT found." << std::endl;
        return NULL;
    }
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

GameObjectManager* GameObjectManager::getInstance() {
    if(P_SHARED_INSTANCE == NULL)
        P_SHARED_INSTANCE = new GameObjectManager();

    return P_SHARED_INSTANCE;
}
/* * * * * * * * * * * * * * * * * * * * */
