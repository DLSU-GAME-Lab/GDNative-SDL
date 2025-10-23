#pragma once
#include "ARenderer.h"
#include "AGameObject.h"
class ColliderRenderer: public ARenderer
{
private:
	SDL_FRect Rect;
public:
	ColliderRenderer(SDL_FRect rect);
	// Inherited via ARenderer
	void perform() override;
};

