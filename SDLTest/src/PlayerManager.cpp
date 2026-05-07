#include "PlayerManager.h"
#include "EmptyObject.h"
#include "GameObjectManager.h"

PlayerManager* PlayerManager::P_SHARED_INSTANCE = NULL;

void PlayerManager::initialize()
{
    EmptyObject* pManagerObject = new EmptyObject("Player Manager");
    P_SHARED_INSTANCE = new PlayerManager();
    pManagerObject->attachComponent(P_SHARED_INSTANCE);
    GameObjectManager::getInstance()->addObject(pManagerObject);
}

void PlayerManager::setPlayer(Player* pPlayer)
{
	this->pPlayer = pPlayer;
}

void PlayerManager::destroy()
{
    delete P_SHARED_INSTANCE->pOwner;
}

PlayerManager* PlayerManager::getInstance()
{
    return P_SHARED_INSTANCE;
}
