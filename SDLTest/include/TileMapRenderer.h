#pragma once
#include "AComponent.h"
#include "Camera.h"
#include <SDL3/SDL.h>
#include <vector>

class TileMapRenderer : public AComponent
{
private:
	SDL_Renderer* pRenderer;
	Camera* pCamera;
	std::vector<std::vector<SDL_Texture*>> vecTile;

	float fWidth;
	float fHeight;

public:
	TileMapRenderer(float fWidth, float fHeight);
	~TileMapRenderer();

	void onAttach() override;
	void perform() override;

	void addTile(Uint64 r, Uint64 c, SDL_Texture* pTile);
	void removeTile(Uint64 r, Uint64 c);
};

