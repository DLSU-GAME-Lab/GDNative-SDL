#pragma once
#include "AComponent.h"
#include "Vector2D.h"

class AGeneralInput : public AComponent
{
protected:
	SDL_Event* eEvent;

public:
	AGeneralInput(std::string strName);
	~AGeneralInput();

	virtual void onAttach() {};
	virtual void perform() = 0;

	void setEvent(SDL_Event* eEvent);

//private:
//	virtual void onMouseHovered(Vector2D mousePos) = 0;
//	virtual void onMouseButtonDown(Uint8 mouseButton) = 0;
//	virtual void onMouseButtonUp(Uint8 mouseButton) = 0;
//	virtual void onKeyPressed() = 0;
//	virtual void onKeyReleased() = 0;
};

