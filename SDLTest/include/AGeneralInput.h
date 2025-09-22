#pragma once
#include "AComponent.h"
#include "Vector2D.h"
#include <SDL3/SDL.h>

class AGeneralInput : public AComponent
{
private:
	SDL_Event* eEvent;

public:
	AGeneralInput(std::string strName);
	~AGeneralInput();

	virtual void perform() = 0;

	void setEvent(SDL_Event* eEvent);

//private:
//	virtual void onMouseHovered(Vector2D mousePos) = 0;
//	virtual void onMouseButtonDown(Uint8 mouseButton) = 0;
//	virtual void onMouseButtonUp(Uint8 mouseButton) = 0;
//	virtual void onKeyPressed() = 0;
//	virtual void onKeyReleased() = 0;
};

