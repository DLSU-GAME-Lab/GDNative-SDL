#include "SDL3/SDL.h"
#include "Vector2D.h"
#include <unordered_map>
#include <unordered_set>
#include <vector>

class InputManager
{
private:
    typedef std::unordered_map<Uint64, SDL_TouchFingerEvent> TouchMap;
    typedef std::unordered_set<SDL_Scancode> KeySet;


private:
    SDL_Event eEvent{};
    float logicalX = 0.0f;
    float logicalY = 0.0f;
	const bool debugTouches = false; // set to true to log touch events
    const bool debugKeyboard = true; // set to true to log keyboard events
    long frame = 0;

    TouchMap activeTouchMap; // track active touches by their ID
    KeySet activeKeySet;

    /* * * * * * * * * * * * * * * * * * * * *
     *       SINGLETON-RELATED CONTENT       *
     * * * * * * * * * * * * * * * * * * * * */
private:
    static InputManager* P_SHARED_INSTANCE;

private:
    InputManager() = default;
    InputManager(const InputManager&) = delete;
    InputManager& operator=(const InputManager&) = delete;

public:
    ~InputManager() = default;

    static void initialize();
    static void destroy();
    static InputManager* getInstance();
    /* * * * * * * * * * * * * * * * * * * * */

public:
    void setLogicalSize(float x, float y);

    // Polls all queued SDL events and updates internal input state.
    void processEvents(SDL_Event* eEvent);

    // Keyboard
    bool isKeyDown(const SDL_Scancode& key) const;

    // Mouse
    bool isMouseButtonDown(Uint8 button) const;
    bool getMousePosition(float& outX, float& outY) const;

    // Touch
    bool hasActiveTouches() const;
    int getActiveTouchCount() const;
    bool isFingerActive(Uint64 fingerId) const;
    bool getFingerPosition(Uint64 fingerId, float& outX, float& outY) const;
    bool getFingerRawPosition(Uint64 fingerId, float& outX, float& outY) const;
    bool isAnyFingerInRect(SDL_FRect fRect, Uint64& fingerId) const;
    std::vector<SDL_TouchFingerEvent> getActiveTouches() const;

private:
    Vector2D rawToScreenPosition(float x, float y) const;
    float toLogicalX(float normalizedX) const;
    float toLogicalY(float normalizedY) const;
};