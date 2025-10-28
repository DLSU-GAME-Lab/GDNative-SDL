#pragma once
#include "SDL3/SDL.h"
#include "Settings.h"
#include <string>
#include "EmptyObject.h"
#include "PhysicsSystem.h"

class Runner
{
private:
    //Window
    SDL_Window* pWindow;
    SDL_Renderer* pRenderer;
    SDL_GPUDevice* pGPUDevice;
    uint64_t ticks;
    float fWindowScale;
    std::string strWindowTitle;


    //FPS
    long lFrameTime;
    static const int MIN_FRAME_TIME = 16;

    unsigned long lFPSTime;
    int nNumOfFPS, nFPS;

    // ImGui init tracking (to avoid double-init / double-shutdown)
    // Defaults set to false to avoid "not initialized" warnings.
    bool imGuiContextCreated = false;
    bool imGuiPlatformInitialized = false;
    bool imGuiRendererInitialized = false;

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
