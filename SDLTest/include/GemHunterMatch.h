#pragma once
#include "AGame.h"
#include <string>

// Gem Hunter Match: the game-specific part of the startup that Runner used to
// contain. Resizable window with letterbox presentation, GPU renderer when
// available, camera that needs the letterbox rect.
class GemHunterMatch : public AGame
{
public:
    SDL_Window* createWindow() override;
    SDL_Renderer* createRenderer(SDL_Window* pWindow) override;
    void setupPresentation(SDL_Window* pWindow, SDL_Renderer* pRenderer) override;
    void initializeSystems(SDL_Window* pWindow, SDL_Renderer* pRenderer) override;
    void registerScenes() override;

    void destroySystems() override;

    void onEvent(SDL_Event* pEvent) override;
    void update(float fDeltaTime) override;

private:
    std::string strWindowTitle;
};
