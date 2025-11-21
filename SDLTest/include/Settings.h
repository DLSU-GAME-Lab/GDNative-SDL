#pragma once
#include "SDL3/SDL.h"
static bool showWidgets = false;
static float gameHeight = 1080;
static float gameWidth = 1920;
static int gameFPS = 60;
static int frameDelay = 1000 / gameFPS;
static SDL_Color colorBlack{ 0,0,0,255 };
static SDL_Color colorWhite{ 255,255,255,255 };
static SDL_Color colorRed{ 255,0,0,255 };
static SDL_Color colorYellow{ 255,255,0,255 };
static SDL_Color colorBlue{ 0,0,255,255 };
static SDL_Color colorCyan{ 0,255,255,255 };
static SDL_Color colorPurple{ 128,0,128,255 };