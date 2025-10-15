#include "Level1Scene.h"
#include "TextureManager.h"
#include "GameObjectManager.h"
#include "CameraManager.h"
#include "PhysicsManager.h"
#include "Player.h"
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
	TextureManager::getInstance()->loadFromFolder("animations/player_idle", "player_idle");
	TextureManager::getInstance()->loadFromFolder("animations/player_run", "player_run");
	TextureManager::getInstance()->loadFromFolder("animations/player_jump", "player_jump");

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
	//PhysicsManager::initialize();

	//Player* pPlayer = new Player(Vector2D(200, 200), Vector2D(), 0.0f);
	//GameObjectManager::getInstance()->addObject(pPlayer);

	TileMap* tileMap = new TileMap("Platforms");
	GameObjectManager::getInstance()->addObject(tileMap);
	TileMapRenderer* pTMR = (TileMapRenderer*)tileMap->findComponentByName("TileMapRenderer");

	std::vector<SDL_Texture*> tile;
	tile.push_back(TextureManager::getInstance()->get("Grass_Tile_TL"));	//0
	tile.push_back(TextureManager::getInstance()->get("Grass_Tile_TC"));	//1
	tile.push_back(TextureManager::getInstance()->get("Grass_Tile_TR"));	//2
	tile.push_back(TextureManager::getInstance()->get("Grass_Tile_ML"));	//3
	tile.push_back(TextureManager::getInstance()->get("Grass_Tile_MC"));	//4
	tile.push_back(TextureManager::getInstance()->get("Grass_Tile_MR"));	//5
	tile.push_back(TextureManager::getInstance()->get("Grass_Tile_BL"));	//6
	tile.push_back(TextureManager::getInstance()->get("Grass_Tile_BC"));	//7
	tile.push_back(TextureManager::getInstance()->get("Grass_Tile_BR"));	//8

	pTMR->setTileSize(512, 512);
	tileMap->setScale(Vector2D(0.2f));

	//left wall
	pTMR->addTile(0, 0, tile[6]);
	pTMR->addTile(0, 1, tile[7]);
	pTMR->addTile(0, 2, tile[7]);
	pTMR->addTile(0, 3, tile[8]);

	for (Uint64 i = 1; i <= 10; i++)
	{
		pTMR->addTile(i, 0, tile[3]);
		pTMR->addTile(i, 1, tile[4]);
		pTMR->addTile(i, 2, tile[4]);
		pTMR->addTile(i, 3, tile[5]);
	}

	//floor
	pTMR->addTile(0, 4, tile[6]);
	pTMR->addTile(1, 4, tile[3]);
	pTMR->addTile(2, 4, tile[0]);

	for (Uint64 i = 5; i < 56; i++)
	{
		pTMR->addTile(0, i, tile[7]);
		pTMR->addTile(1, i, tile[4]);
		pTMR->addTile(2, i, tile[1]);
	}

	CameraManager::getInstance()->getCurrentCamera()->setPos(Vector2D(800, 500));
}

void Level1Scene::onUnloadResources()
{
	TextureManager::getInstance()->unload("player_idle");
	TextureManager::getInstance()->unload("player_run");
	TextureManager::getInstance()->unload("player_jump");

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
