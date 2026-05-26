#include "InteractButtonController.h"
#include "GameObjectManager.h"
#include "InputManager.h"
#include "PlayerManager.h"
#include "ButtonInput.h"
#include "AGameObject.h"
#include <vector>
#include <string>
#include <iostream>
static PlayerInput* findPlayerInputSafe()
{
    GameObjectManager* gm = GameObjectManager::getInstance();
    if (!gm) return nullptr;

    AGameObject* pPlayer = GameObjectManager::getInstance()->findObjectByName("Player");
    if (pPlayer != nullptr)
    {
        // use findComponentByName (returns AComponent*) then dynamic_cast
        AComponent* comp = pPlayer->findComponentByName("PlayerInput");
        PlayerInput* input = dynamic_cast<PlayerInput*>(comp);
        if (input) return input;
    }
    return nullptr;
}

InteractButtonController::InteractButtonController() :AComponent("InteractButtonController", ComponentType::SCRIPT)
{
}

void InteractButtonController::onAttach()
{
    // Try to resolve player input on attach (fast path)
    PlayerInput* p = findPlayerInputSafe();
    if (p)
    {
        // store in owner component's userdata (or a static) — keep simple: store on this instance if header has member
        this->playerInput = p;
    }

    this->pRenderer = (SpriteRenderer*)this->pOwner->findComponentByName("SpriteRenderer");
}

void InteractButtonController::perform()
{
  
        Uint64 fingerId = 0;
        float x, y = 0.0f;
        bool isAnyFingerInRect = InputManager::getInstance()->isAnyFingerInRect(this->pRenderer->getRect(), fingerId);

        if (isAnyFingerInRect && !this->playerInput->getInteracted())
        {
            this->playerInput->setInteracted(true);
            SDL_Log("Virtual Interacted");
        }
        else if (!isAnyFingerInRect && this->playerInput->getInteracted()) this->playerInput->setInteracted(false);
    
}
