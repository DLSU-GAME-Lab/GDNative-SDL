#pragma once
#include "ARenderer.h"
#include "Camera.h"
#include <SDL3/SDL.h>
#include <vector>

class TileMapRenderer : public ARenderer
{
private:
	std::vector<std::vector<SDL_Texture*>> vecTile;

	float fTileWidth;
	float fTileHeight;

public:
	TileMapRenderer(float fTileWidth = 1.0f, float fTileHeight = 1.0f);
	~TileMapRenderer();

	void onAttach() override;
	void perform() override;

	void addTile(Uint64 r, Uint64 c, SDL_Texture* pTile);
	void removeTile(Uint64 r, Uint64 c);

	void setTileSize(float fTileWidth, float fTileHeight);
};

