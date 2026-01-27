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

    std::vector<AGameObject*>& objs = gm->getAllObjects();
    for (AGameObject* obj : objs)
    {
        if (!obj) continue;
        if (obj->getName() == "Player")
        {
            // use findComponentByName (returns AComponent*) then dynamic_cast
            AComponent* comp = obj->findComponentByName("PlayerInput");
            if (!comp) continue;
            PlayerInput* input = dynamic_cast<PlayerInput*>(comp);
            if (input) return input;
        }
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
    // lazy-resolve if not found yet
    if (!this->playerInput)
    {
        this->playerInput = findPlayerInputSafe();
    }

    auto btn = (ButtonInput*)pOwner->findComponentByName("ButtonInput");
    if (!btn || !btn->getClicked()) return;

    if (!this->playerInput)
    {
        // no player yet; ignore click (safe: no redundant map insertions)
        btn->setClicked(false); // consume so it doesn't retrigger repeatedly
        return;
    }

    this->playerInput->setVirtualJump(true);
    btn->setClicked(false); // consume click
}