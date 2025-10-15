#include "Level1Scene.h"
#include "TextureManager.h"
#include "GameObjectManager.h"
#include "CameraManager.h"
#include "TileMap.h"
#include "TileMapRenderer.h"

Level1Scene::Level1Scene() : AScene(SceneTag::LEVEL_1_SCENE)
{

}

Level1Scene::~Level1Scene()
{

}

void Level1Scene::onLoadResources()
{
	TextureManager::getInstance()->load("tilemaps/bottom.png", "Grass_Tile_BC");
	TextureManager::getInstance()->load("tilemaps/bottomleft.png", "Grass_Tile_BL");
	TextureManager::getInstance()->load("tilemaps/bottomright.png", "Grass_Tile_BR");
	TextureManager::getInstance()->load("tilemaps/dirt.png", "Grass_Tile_MC");
	TextureManager::getInstance()->load("tilemaps/left.png", "Grass_Tile_ML");
	TextureManager::getInstance()->load("tilemaps/right.png", "Grass_Tile_MR");
	TextureManager::getInstance()->load("tilemaps/top.png", "Grass_Tile_TC");
	TextureManager::getInstance()->load("tilemaps/topleft.png", "Grass_Tile_TL");
	TextureManager::getInstance()->load("tilemaps/topright.png", "Grass_Tile_TR");
}

void Level1Scene::onLoadObjects()
{
	TileMap* tileMap = new TileMap("Platforms");
	GameObjectManager::getInstance()->addObject(tileMap);
	TileMapRenderer* pTMR = (TileMapRenderer*)tileMap->findComponentByName("TileMapRenderer");

	SDL_Texture* pTileBL = TextureManager::getInstance()->get("Grass_Tile_BL");

	pTMR->setTileSize(512, 512);
	tileMap->setScale(Vector2D(0.2f));

	for (Uint64 r = 0; r < 5; r++)
	{
		for (Uint64 c = 0; c < 5; c++)
		{
			pTMR->addTile(r, c, pTileBL);
		}
	}

	CameraManager::getInstance()->getCurrentCamera()->setPos(Vector2D(800, 500));
}

void Level1Scene::onUnloadResources()
{
	TextureManager::getInstance()->unload("Grass_Tile_BC");
	TextureManager::getInstance()->unload("Grass_Tile_BL");
	TextureManager::getInstance()->unload("Grass_Tile_BR");
	TextureManager::getInstance()->unload("Grass_Tile_MC");
	TextureManager::getInstance()->unload("Grass_Tile_ML");
	TextureManager::getInstance()->unload("Grass_Tile_MR");
	TextureManager::getInstance()->unload("Grass_Tile_TC");
	TextureManager::getInstance()->unload("Grass_Tile_TL");
	TextureManager::getInstance()->unload("Grass_Tile_TR");
}
