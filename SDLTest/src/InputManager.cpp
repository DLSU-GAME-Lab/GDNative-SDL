#include "InputManager.h"

InputManager* InputManager::P_SHARED_INSTANCE = nullptr;

void InputManager::initialize()
{
    if (!P_SHARED_INSTANCE)
    {
        P_SHARED_INSTANCE = new InputManager();
    }
}

void InputManager::destroy()
{
    delete P_SHARED_INSTANCE;
    P_SHARED_INSTANCE = nullptr;
}

InputManager* InputManager::getInstance()
{
    return P_SHARED_INSTANCE;
}

void InputManager::setLogicalSize(float x, float y)
{
    logicalX = x;
    logicalY = y;
}

void InputManager::processEvents(SDL_Event* eEvent)
{
    switch (eEvent->type)
    {
    case SDL_EVENT_FINGER_DOWN:
    {
        activeTouches[eEvent->tfinger.fingerID] = eEvent->tfinger;
        if (debugTouches) {
            SDL_Log("[InputManager] Finger down: ID=%" SDL_PRIu64 " at (%.2f, %.2f) with pressure %.2f",
                eEvent->tfinger.fingerID, eEvent->tfinger.x, eEvent->tfinger.y, eEvent->tfinger.pressure);
        }
        break;
    }

    case SDL_EVENT_FINGER_MOTION:
    {
        activeTouches[eEvent->tfinger.fingerID] = eEvent->tfinger;
        if (debugTouches) {
            SDL_Log("[InputManager] Finger motion: ID=%" SDL_PRIu64 " at (%.2f, %.2f) with pressure %.2f",
                eEvent->tfinger.fingerID, eEvent->tfinger.x, eEvent->tfinger.y, eEvent->tfinger.pressure);
        }
        break;
    }

    case SDL_EVENT_FINGER_UP:
    case SDL_EVENT_FINGER_CANCELED:
    {
        activeTouches.erase(eEvent->tfinger.fingerID);
        if (debugTouches) {
            SDL_Log("[InputManager] Finger up/canceled: ID=%" SDL_PRIu64, eEvent->tfinger.fingerID);
        }
        break;
    }

    default:
        break;
    }
}

bool InputManager::isKeyDown(SDL_Scancode scancode) const
{
    const bool* keyboardState = SDL_GetKeyboardState(nullptr);
    return keyboardState && keyboardState[scancode];
}

bool InputManager::isMouseButtonDown(Uint8 button) const
{
    SDL_MouseButtonFlags mouseState = SDL_GetMouseState(nullptr, nullptr);
    return (mouseState & SDL_BUTTON_MASK(button)) != 0;
}

bool InputManager::getMousePosition(float& outX, float& outY) const
{
    float x = 0.0f;
    float y = 0.0f;

    if (!SDL_GetMouseState(&x, &y))
    {
        outX = x;
        outY = y;
        return true;
    }

    outX = x;
    outY = y;
    return true;
}

bool InputManager::hasActiveTouches() const
{
    return !activeTouches.empty();
}

int InputManager::getActiveTouchCount() const
{
    return static_cast<int>(activeTouches.size());
}

bool InputManager::isFingerActive(Uint64 fingerId) const
{
    return activeTouches.find(fingerId) != activeTouches.end();
}

bool InputManager::getFingerPosition(Uint64 fingerId, float& outX, float& outY) const
{
    auto it = activeTouches.find(fingerId);
    if (it == activeTouches.end())
    {
        return false;
    }

    outX = toLogicalX(it->second.x);
    outY = toLogicalY(it->second.y);
    return true;
}

std::vector<SDL_TouchFingerEvent> InputManager::getActiveTouches() const
{
    std::vector<SDL_TouchFingerEvent> touches;
    touches.reserve(activeTouches.size());

    for (const auto& pair : activeTouches)
    {
        touches.push_back(pair.second);
    }

    return touches;
}

float InputManager::toLogicalX(float normalizedX) const
{
    if (logicalX <= 0.0f)
    {
        return normalizedX;
    }
    return normalizedX * logicalX;
}

float InputManager::toLogicalY(float normalizedY) const
{
    if (logicalY <= 0.0f)
    {
        return normalizedY;
    }
    return normalizedY * logicalY;
}