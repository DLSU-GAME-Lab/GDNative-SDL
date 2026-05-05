#include "JumpButtonController.h"
#include "GameObjectManager.h"
#include "ButtonInput.h"
#include "AGameObject.h"
#include <vector>
#include <string>
#include <iostream>

// Helper: safe search for PlayerInput by scanning active game objects
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

JumpButtonController::JumpButtonController()
        : AComponent("JumpButtonController", ComponentType::SCRIPT)
{
}

void JumpButtonController::onAttach()
{
    // Try to resolve player input on attach (fast path)
    PlayerInput* p = findPlayerInputSafe();
    if (p)
    {
        // store in owner component's userdata (or a static) — keep simple: store on this instance if header has member
        this->playerInput = p;
    }
}

// perform() is called each frame / per event; only search once if not found
void JumpButtonController::perform()
{
    auto btn = (ButtonInput*)pOwner->findComponentByName("ButtonInput");
    if (!btn) return;

    if (this->playerInput)
    {
        this->playerInput->setVirtualJump(btn->getClicked());
    }

    btn->setClicked(false); // consume click
}