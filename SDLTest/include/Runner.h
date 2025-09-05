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


};

Runner::Runner()
{
}

Runner::~Runner()
{
}