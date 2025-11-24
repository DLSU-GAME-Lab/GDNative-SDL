#pragma once
#include "AGameObject.h"

class TileMap : public AGameObject
{
private:
	float fTileWidth;
	float fTileHeight;

public:
	TileMap(std::string strName);
	
	void initialize() override;

};

