#pragma once

#include "AGameObject.h"
#include "SpriteRenderer.h"
#include <SDL3/SDL.h>

class AButton : public AGameObject
{
private:
	SpriteRenderer* pSprite;

public:
	AButton(std::string strName);
	~AButton();
	void processInput(SDL_Event eEvent);

	virtual void OnPressed(SDL_MouseButtonEvent eMouseEvent) = 0;
	virtual void OnReleased(SDL_MouseButtonEvent eMouseEvent) = 0;
	virtual void OnHovered() = 0;

private:
	bool contains(float fX, float fY);
};

