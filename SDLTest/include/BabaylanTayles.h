#pragma once
#include "AGame.h"
#include <string>

// Babaylan Tayles: the game-specific part of the startup that Runner used to
// contain. Fullscreen window scaled to the display, plain SDL renderer, camera
// that follows the window size and scale, input manager and event broadcaster.
class BabaylanTayles : public AGame
{
public:
    SDL_Window* createWindow() override;
    SDL_Renderer* createRenderer(SDL_Window* pWindow) override;
    void setupPresentation(SDL_Window* pWindow, SDL_Renderer* pRenderer) override;
    void initializeSystems(SDL_Window* pWindow, SDL_Renderer* pRenderer) override;
    void registerScenes() override;

    void destroySystems() override;

    void onEvent(SDL_Event* pEvent) override;
    void processEvent(SDL_Event* pEvent) override;
    void update(float fDeltaTime) override;

private:
    std::string strWindowTitle;
    SDL_Window* pWindow = nullptr;
    float scaleX = 1.0f;
    float scaleY = 1.0f;
};
