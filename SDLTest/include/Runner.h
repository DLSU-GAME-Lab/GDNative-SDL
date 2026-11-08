#pragma once
#include "SDL3/SDL.h"
#include "Settings.h"
#include <string>
#include "EmptyObject.h"
#include "PhysicsSystem.h"

class AGame;

// Identical in Babaylan Tayles and Gem Hunter Match. Everything that differs
// between the games lives in the game's AGame subclass.
class Runner
{
private:
    //Window
    SDL_Window* pWindow = nullptr;
    SDL_Renderer* pRenderer = nullptr;   // GPU renderer (owns its GPU device) or plain fallback renderer
    AGame* pGame = nullptr;        // the game's own class, owned

public:
    Runner();
    ~Runner();

public:
    void run();

private:
    void processEvents(SDL_Event* eEvent);
    void update(float fDeltaTime);
    void render();

private:
    void registerScenes();
};
