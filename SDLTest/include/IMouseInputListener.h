#pragma once
#include "Vector2D.h"
#include <SDL3/SDL.h>

class IMouseInputListener
{
public:
	virtual void onMouseHovered(Vector2D mousePos) = 0;
	virtual void onMouseButtonDown(Uint8 mouseButton) = 0;
	virtual void onMouseButtonUp(Uint8 mouseButton) = 0;

	virtual SDL_FRect getRect() const = 0;
};
