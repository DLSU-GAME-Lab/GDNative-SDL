#include "PlayerManager.h"
#include "EmptyObject.h"
#include "GameObjectManager.h"
#include "InputManager.h"

void PlayerManager::perform()
{
    bool pressingSpace = InputManager::getInstance()->isKeyDown(SDL_SCANCODE_F);
    bool pressingA = InputManager::getInstance()->isKeyDown(SDL_SCANCODE_A);
    bool pressingD = InputManager::getInstance()->isKeyDown(SDL_SCANCODE_D);
//
//    if (pressingSpace)
//        pController->Jump(this->fJumpForce);

    if (pressingA || pressingD) SDL_Log("Moving");

    if (pressingA && !pressingD) pController->Move(-1.0f);
    else if (pressingD && ! pressingA) pController->Move(1.0f);
    //else pController->Move(Vector2D::Zero());
}

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
    this->pController = (PlayerController*)pPlayer->findComponentByName("PlayerController");
}

void PlayerManager::destroy()
{
    delete P_SHARED_INSTANCE->pOwner;
}

PlayerManager* PlayerManager::getInstance()
{
    return P_SHARED_INSTANCE;
}

PlayerController* PlayerManager::getPlayerController() const {
    return this->pController;
}
