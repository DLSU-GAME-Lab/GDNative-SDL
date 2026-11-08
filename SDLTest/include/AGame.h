#pragma once
#include <SDL3/SDL.h>
#include "EnumSceneTag.h"

// ---------------------------------------------------------------------------
// AGame: everything that is different between the games that share Runner.
//
// Runner owns the SDL init, the main loop, the timing and the render order, and
// only ever talks to an AGame. The game's own class (GemHunterMatch,
// BabaylanTayles, ...) overrides the rest: window and renderer, which systems
// exist (and in which order they are created and destroyed), the scenes, and
// how input and updates are routed.
//
// Each game's project compiles exactly one subclass, which says so once with
// IMPLEMENT_GAME(...) at the bottom of its .cpp. Runner calls AGame::create(),
// so it never names or constructs a game itself.
// ---------------------------------------------------------------------------
class AGame
{
public:
    virtual ~AGame() {}

    // Defined by the game's own class through IMPLEMENT_GAME.
    static AGame* create();

    // --- startup, called by Runner in this order ---
    virtual SDL_Window* createWindow() = 0;
    virtual SDL_Renderer* createRenderer(SDL_Window* pWindow) = 0;
    virtual void setupPresentation(SDL_Window* pWindow, SDL_Renderer* pRenderer) = 0;
    virtual void initializeSystems(SDL_Window* pWindow, SDL_Renderer* pRenderer) = 0;
    virtual void registerScenes() = 0;
    virtual SceneTag getStartScene() const { return SceneTag::TITLE_SCENE; }

    // --- shutdown, called by Runner after the scene is unloaded ---
    virtual void destroySystems() = 0;

    // --- per frame ---
    // Sees every event before Runner handles it (resize, touch logging, ...).
    virtual void onEvent(SDL_Event* pEvent) {}
    // Routes an event to the game. Default: GameObjectManager::processInput.
    virtual void processEvent(SDL_Event* pEvent);
    virtual void update(float fDeltaTime) = 0;

protected:
    // MetricsManager setup shared by the games, to be called from
    // initializeSystems() at the point where it should be created.
    void initializeMetrics();
};

// Put once at the bottom of a game's .cpp: tells Runner which class to create.
#define IMPLEMENT_GAME(GameClass) AGame* AGame::create() { return new GameClass(); }
