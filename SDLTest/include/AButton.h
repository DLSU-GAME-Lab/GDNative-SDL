#pragma once

#include "AGameObject.h"
#include "SpriteRenderer.h"
#include <SDL3/SDL.h>

class AButton : public AGameObject
{
protected:
	SpriteRenderer* pSprite;

protected:
	bool contains(float fX, float fY);

public:
	AButton(std::string strName);
	virtual ~AButton();

	void processInput(SDL_Event* eEvent);

	virtual void OnPressed(SDL_MouseButtonEvent eMouseEvent) = 0;
	virtual void OnReleased(SDL_MouseButtonEvent eMouseEvent) = 0;
	virtual void OnHovered() = 0;
};

