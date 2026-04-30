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

	virtual void perform() = 0;

	void setEvent(SDL_Event* eEvent);


};

