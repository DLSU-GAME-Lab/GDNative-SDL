#include "VirtualJoystick.h"
#include "GameObjectManager.h"
#include "PlayerManager.h"
#include "PlayerInput.h"
#include "AGameObject.h"
#include "ButtonInput.h"
#include "Sprite.h"
#include "InputManager.h"
#include <algorithm>
#include <cmath>
#include <vector>
#include <iostream>

// Helper: safe scan for PlayerInput (same idea as jump controller)
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
            AComponent* comp = obj->findComponentByName("PlayerInput");
            if (!comp) continue;
            PlayerInput* input = dynamic_cast<PlayerInput*>(comp);
            if (input) return input;
        }
    }
    return nullptr;
}

VirtualJoystick::VirtualJoystick(float radius)
        : AComponent("VirtualJoystick", ComponentType::SCRIPT), radius(radius), active(false), playerInput(nullptr)
{
}

void VirtualJoystick::onAttach()
{
    // reset thumb local pos to zero if exists
    if (auto child = this->pOwner->findChildByName("VirtualJoystickThumb"))
    {
        pThumb = dynamic_cast<Sprite*>(child);
        if (pThumb) {
            pThumb->setLocalPos(Vector2D(0.0f, 0.0f));
            SDL_Log("Joystick Thumb Assigned");
        }
    }

    this->pBase = (SpriteRenderer*)this->pOwner->findComponentByName("SpriteRenderer");

    // try to resolve player input early
    playerInput = findPlayerInputSafe();
}

void VirtualJoystick::perform()
{
    // lazy-resolve if not found yet
    if (!playerInput)
        playerInput = findPlayerInputSafe();

    // still okay to continue even if playerInput is null (no crash)
    // but joystick won't drive movement until input exists.
    if (!playerInput && !active)
    {
        // we can still process DOWN to become active if desired, but without playerInput it won't move the player
        // proceed (we won't return early) so thumb can still visually update if needed
    }

    center = this->pOwner->getPos();

    Uint64 fingerId = 0;
    float x, y = 0.0f;
    bool isAnyFingerInRect = InputManager::getInstance()->isAnyFingerInRect(this->pBase->getRect(), fingerId);
    bool getFingerPosition = InputManager::getInstance()->getFingerPosition(fingerId, x, y);

    if (isAnyFingerInRect) active = true;
    else if (!InputManager::getInstance()->hasActiveTouches() && active)
    {
        active = false;
        reset = true;
    }

    if (active)
    {
        Vector2D pos = Vector2D(x, y);
        updateFromTouch(pos);
    }
    else if (reset)
    {
        reset = false;
        resetTouch();
    }
}

void VirtualJoystick::updateFromTouch(const Vector2D& pos)
{
    Vector2D delta = pos - center;
    //SDL_Log("Pos: (%.2f, %.2f), center: (%.2f, %.2f)", pos.x, pos.y, center.x, center.y);

    float len = std::sqrt(delta.x * delta.x + delta.y * delta.y);

    float clampedLen = std::min(len, radius);
    // normalized direction, safe divide
    float safeLen = std::max(len, 0.0001f);
    Vector2D dir = { delta.x / safeLen, delta.y / safeLen };
    Vector2D virt = { dir.x * (clampedLen / radius), dir.y * (clampedLen / radius) };

    // joystick is horizontal only
    virt.y = 0.0f;
    virt.x = std::clamp(virt.x, -1.0f, 1.0f);

    PlayerManager::getInstance()->getPlayerController()->Move(virt.x);
    // update thumb visual (local position inside joystick)
    if (pThumb) pThumb->setLocalPos(Vector2D(dir.x * (clampedLen * 0.6f), dir.y * (clampedLen * 0.6f)));
    else SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Joystick Thumb not found.");
}

void VirtualJoystick::resetTouch()
{
    updateFromTouch(this->pOwner->getPos());
    if (pThumb) pThumb->setLocalPos(Vector2D(0.0f, 0.0f));
}
