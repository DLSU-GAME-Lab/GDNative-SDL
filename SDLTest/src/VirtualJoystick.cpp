#include "VirtualJoystick.h"
#include "GameObjectManager.h"
#include "PlayerInput.h"
#include "AGameObject.h"
#include "ButtonInput.h"
#include "Sprite.h"
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
        : AGeneralInput("VirtualJoystick"), radius(radius), active(false), playerInput(nullptr)
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

    // DOWN: set active only if touch/mouse is inside radius (use logical coords)
    if (eEvent && (eEvent->type == SDL_EVENT_FINGER_DOWN || eEvent->type == SDL_EVENT_MOUSE_BUTTON_DOWN))
    {
        Vector2D pos(this->logicalX, this->logicalY);
        Vector2D d = pos - center;
        float dist = std::sqrt(d.x * d.x + d.y * d.y);
        if (dist <= radius) active = true;
    }

    // UP: release and reset movement + thumb
    if (eEvent && (eEvent->type == SDL_EVENT_FINGER_UP || eEvent->type == SDL_EVENT_MOUSE_BUTTON_UP))
    {
        active = false;
        if (playerInput) playerInput->setVirtualMovement(Vector2D::Zero());
        if (pThumb) pThumb->setLocalPos(Vector2D(0.0f, 0.0f));
        return;
    }

    // motion: update only if active
    if (active && eEvent && (eEvent->type == SDL_EVENT_FINGER_MOTION || eEvent->type == SDL_EVENT_MOUSE_MOTION))
    {
        Vector2D pos(this->logicalX, this->logicalY);
        updateFromTouch(pos);
    }
}

void VirtualJoystick::updateFromTouch(const Vector2D& pos)
{
    Vector2D delta = pos - center;

    float len = std::sqrt(delta.x * delta.x + delta.y * delta.y);

    float clampedLen = std::min(len, radius);
    // normalized direction, safe divide
    float safeLen = std::max(len, 0.0001f);
    Vector2D dir = { delta.x / safeLen, delta.y / safeLen };
    Vector2D virt = { dir.x * (clampedLen / radius), dir.y * (clampedLen / radius) };

    // joystick is horizontal only
    virt.y = 0.0f;
    virt.x = std::clamp(virt.x, -1.0f, 1.0f);

    if (playerInput) playerInput->setVirtualMovement(virt);

    // update thumb visual (local position inside joystick)
    if (pThumb) pThumb->setLocalPos(Vector2D(dir.x * (clampedLen * 0.6f), dir.y * (clampedLen * 0.6f)));
    else SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Joystick Thumb not found.");
}