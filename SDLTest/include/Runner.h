#pragma once
#include "SDL3/SDL.h"
class Runner
{
private:
	static SDL_Window* window;
	static SDL_Renderer* renderer;
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
    void updateBounds(SDL_EventType& EEvent);
};
