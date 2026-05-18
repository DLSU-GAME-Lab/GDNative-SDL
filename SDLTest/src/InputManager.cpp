#include "InputManager.h"
#include "CameraManager.h"

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

void InputManager::processEvents(SDL_Event* e)
{
    switch (e->type)
    {

    case SDL_EVENT_KEY_DOWN:
    {
        if (!this->activeKeySet.contains(e->key.scancode) && !e->key.repeat)
        {
            this->activeKeySet.insert(e->key.scancode);
            this->frame++;
            if (debugKeyboard) SDL_Log("Key Down: %c, %ld", e->key.key, frame);
        }
        break;
    }

    case SDL_EVENT_KEY_UP:
    {
        if (this->activeKeySet.contains(e->key.scancode))
        {
            this->activeKeySet.erase(e->key.scancode);
            this->frame++;
            if (debugKeyboard) SDL_Log("Key Up: %c", e->key.key);
        }
        break;
    }

    case SDL_EVENT_FINGER_DOWN:
    {
        activeTouchMap[e->tfinger.fingerID] = e->tfinger;
        if (debugTouches) {
            SDL_Log("[InputManager] Finger down: ID=%" SDL_PRIu64 " at (%.2f, %.2f) with pressure %.2f",
                e->tfinger.fingerID, e->tfinger.x, e->tfinger.y, e->tfinger.pressure);
        }
        break;
    }

    case SDL_EVENT_FINGER_MOTION:
    {
        activeTouchMap[e->tfinger.fingerID] = e->tfinger;
        if (debugTouches) {
            SDL_Log("[InputManager] Finger motion: ID=%" SDL_PRIu64 " at (%.2f, %.2f) with pressure %.2f",
                e->tfinger.fingerID, e->tfinger.x, e->tfinger.y, e->tfinger.pressure);
        }
        break;
    }

    case SDL_EVENT_FINGER_UP:
    case SDL_EVENT_FINGER_CANCELED:
    {
        activeTouchMap.erase(e->tfinger.fingerID);
        if (debugTouches) {
            SDL_Log("[InputManager] Finger up/canceled: ID=%" SDL_PRIu64, e->tfinger.fingerID);
        }
        break;
    }

    default:
        break;
    }
}

bool InputManager::isKeyDown(const SDL_Scancode& key) const
{
    bool isKeyDown = this->activeKeySet.contains(key);
    return isKeyDown;
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
    return !activeTouchMap.empty();
}

int InputManager::getActiveTouchCount() const
{
    return static_cast<int>(activeTouchMap.size());
}

bool InputManager::isFingerActive(Uint64 fingerId) const
{
    return activeTouchMap.find(fingerId) != activeTouchMap.end();
}

bool InputManager::getFingerPosition(Uint64 fingerId, float& outX, float& outY) const
{
    auto it = activeTouchMap.find(fingerId);
    if (it == activeTouchMap.end())
    {
        return false;
    }

    Vector2D windowPos = rawToScreenPosition(it->second.x, it->second.y);
    Vector2D windowOffset = CameraManager::getInstance()->getWindowOffset();

    outX = windowPos.x;
    outY = windowPos.y;
    return true;
}

bool InputManager::getFingerRawPosition(Uint64 fingerId, float &outX, float &outY) const {
    auto it = activeTouchMap.find(fingerId);
    if (it == activeTouchMap.end())
    {
        return false;
    }

    outX = toLogicalX(it->second.x);
    outY = toLogicalY(it->second.y);
    return true;
}

bool InputManager::isAnyFingerInRect(SDL_FRect fRect, Uint64& fingerId) const
{
    for (auto touch : getActiveTouches())
    {
        Vector2D windowPos = rawToScreenPosition(touch.x, touch.y);
        SDL_FPoint point = {windowPos.x, windowPos.y};
        fingerId = touch.fingerID;
        //SDL_Log("Checked finger: %lu at: (%.2f, %.2f) against rect at: (%.2f, %.2f)",
        //        fingerId, windowPos.x, windowPos.y, fRect.x, fRect.y);
        if (SDL_PointInRectFloat(&point, &fRect))
            return true;
    }

    return false;
}

std::vector<SDL_TouchFingerEvent> InputManager::getActiveTouches() const
{
    std::vector<SDL_TouchFingerEvent> touches;
    touches.reserve(activeTouchMap.size());

    for (const auto& pair : activeTouchMap)
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

Vector2D InputManager::rawToScreenPosition(float x, float y) const
{
    Vector2D windowSize = CameraManager::getInstance()->getWindowSize();
    Vector2D windowOffset = CameraManager::getInstance()->getWindowOffset();
    //SDL_Log("Window Size: %.2f, %.2f", windowSize.x, windowSize.y);
    return Vector2D(x * windowSize.x, y * windowSize.y) - windowOffset;
}
