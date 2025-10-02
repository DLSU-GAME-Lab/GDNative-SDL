#pragma once
#include "Grid.h"
#include <SDL3/SDL.h>

class TileMapRenderer : public Grid
{
public:
	TileMapRenderer(Uint64 w, Uint64 h, float fCellSize);
	~TileMapRenderer();

	void onAttach() override;
	void perform() override;

	void addTile(Uint64 w, Uint64 h, SDL_Texture* pTile);
	void removeTile(Uint64 w, Uint64 h);
};

