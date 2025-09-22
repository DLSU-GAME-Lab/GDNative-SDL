#include "GemManager.h"
#include "GameObjectManager.h"
#include "EmptyObject.h"

void GemManager::perform()
{

}

void GemManager::startLevel()
{
    for (int i = 0; i < this->vecTile.size(); i++)
    {
        //TODO: create random gem
        this->vecGem.push_back(NULL);
    }
}

void GemManager::moveGems()
{
    Vector2D pos = vecGem[selected[0]]->getPos();
    vecGem[selected[0]]->setPos(vecGem[selected[1]]->getPos());
    vecGem[selected[1]]->setPos(pos);
    selected[0] = -1;
    selected[1] = -1;

    matchGems();
}

void GemManager::matchGems()
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
