#include "TileMap.h"
#include "TileMapRenderer.h"

TileMap::TileMap(std::string strName) : AGameObject(strName)
{
	this->fTileWidth = fTileWidth;
	this->fTileHeight = fTileHeight;
}

void TileMap::initialize()
{
	TileMapRenderer* pRenderer = new TileMapRenderer();
	this->attachComponent(pRenderer);
}
