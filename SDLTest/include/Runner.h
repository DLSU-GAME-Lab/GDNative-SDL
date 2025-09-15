#pragma once
#include "SDL3/SDL.h"
#include "Settings.h"
#include <string>

class Runner
{
private:
    //Window
    SDL_Window* pWindow;
    SDL_Renderer* pRenderer;
    uint64_t ticks;
    float fWindowScale;
    std::string strWindowTitle;


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
    void processEvents(SDL_Event eEvent);
    void update();
    void render();

private:
    void registerScenes();
};
