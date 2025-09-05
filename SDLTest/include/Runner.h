#pragma once
#include "SDL3/SDL.h"
#include "Settings.h"
class Runner
{
private:
    //Window
    SDL_Window* pWindow;
    SDL_Renderer* pRenderer;
    SDL_Event* pMainEvent;
    uint64_t ticks;


    //FPS
    long lFrameTime;
    static const int MIN_FRAME_TIME = 16;

    unsigned long lFPSTime;
    int nNumOfFPS, nFPS;



public:
    Runner();
    ~Runner();

public:
    void run();

private:
    void processEvents();
    void update();
    void render();

private:
    //void updateBounds(SDL_EventType EEvent);
};
