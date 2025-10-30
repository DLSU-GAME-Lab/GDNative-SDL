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
	Vector2D offset;

public:
	TileMapRenderer(float fTileWidth = 1.0f, float fTileHeight = 1.0f);
	~TileMapRenderer();

	void onAttach() override;
	void perform() override;
	void drawWidget() override;

	void addTile(Uint64 c, Uint64 r, SDL_Texture* pTile);

	void setTileSize(float fTileWidth, float fTileHeight);
	void setOffset(Vector2D offset);

	Vector2D getCellSize();
	Vector2D getTilePosition(Uint64 c, Uint64 r);
};

