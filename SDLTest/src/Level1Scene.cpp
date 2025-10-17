#include "Level1Scene.h"
#include "TextureManager.h"
#include "GameObjectManager.h"
#include "CameraManager.h"
#include "PhysicsManager.h"
#include "Player.h"
#include "TileMap.h"
#include "TileMapRenderer.h"
#include "AnimatedSprite.h"
#include "SpriteAnimator.h"
#include "Background.h"
#include "Platform.h"
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

	TextureManager::getInstance()->load("platformer/trees.png", "Trees_BG");
	TextureManager::getInstance()->load("tilemaps/bottom.png", "Grass_Tile_BC");
	TextureManager::getInstance()->load("tilemaps/bottomleft.png", "Grass_Tile_BL");
	TextureManager::getInstance()->load("tilemaps/bottomright.png", "Grass_Tile_BR");
	TextureManager::getInstance()->load("tilemaps/dirt.png", "Grass_Tile_MC");
	TextureManager::getInstance()->load("tilemaps/left.png", "Grass_Tile_ML");
	TextureManager::getInstance()->load("tilemaps/right.png", "Grass_Tile_MR");
	TextureManager::getInstance()->load("tilemaps/top.png", "Grass_Tile_TC");
	TextureManager::getInstance()->load("tilemaps/topleft.png", "Grass_Tile_TL");
	TextureManager::getInstance()->load("tilemaps/topright.png", "Grass_Tile_TR");
	TextureManager::getInstance()->load("tilemaps/cornergrass1.png", "Grass_Tile_C1");
	TextureManager::getInstance()->load("tilemaps/cornergrass2.png", "Grass_Tile_C2");
	TextureManager::getInstance()->load("tilemaps/cornergrass3.png", "Grass_Tile_C3");
	TextureManager::getInstance()->load("tilemaps/cornergrass4.png", "Grass_Tile_C4");
	TextureManager::getInstance()->load("tilemaps/grass_platform.png", "Grass_Platform");
}

void Level1Scene::onLoadObjects()
{
	CameraManager::getInstance()->getCurrentCamera()->setPos(Vector2D(1350, 700));

	PhysicsManager::initialize();

	//Player* pPlayer = new Player(Vector2D(200, 200), Vector2D(), 0.0f);
	//GameObjectManager::getInstance()->addObject(pPlayer);
	
	Background* pBackground = new Background("Trees_BG", "Trees_BG", 0.5f);
	pBackground->setPos(Vector2D(0.0f, -200.0f));
	GameObjectManager::getInstance()->addObject(pBackground);

	TileMap* tileMap = new TileMap("Platforms");
	GameObjectManager::getInstance()->addObject(tileMap);
	TileMapRenderer* pTMR = (TileMapRenderer*)tileMap->findComponentByName("TileMapRenderer");

	Player* pPlayer = new Player(Vector2D(1000, 380), Vector2D(0.6f, 0.6f), 0.f);
	GameObjectManager::getInstance()->addObject(pPlayer);
	Animation* pAnim = new Animation("idle", TextureManager::getInstance()->getTexture("player_idle"), 12, AnimationType::LOOP);
	SpriteAnimator* pSpriteAnim = (SpriteAnimator*)pPlayer->findComponentByName("SpriteAnimator");
	pSpriteAnim->addAnimation(pAnim);
	pSpriteAnim->play("idle");

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
	tile.push_back(TextureManager::getInstance()->get("Grass_Tile_C1"));	//9
	tile.push_back(TextureManager::getInstance()->get("Grass_Tile_C2"));	//10
	tile.push_back(TextureManager::getInstance()->get("Grass_Tile_C3"));	//11
	tile.push_back(TextureManager::getInstance()->get("Grass_Tile_C4"));	//12
	tile.push_back(TextureManager::getInstance()->get("Grass_Platform"));	//13

	pTMR->setTileSize(512, 512);
	pTMR->setOffset(Vector2D(0.5f));
	tileMap->setScale(Vector2D(0.2f));

	//left wall
	pTMR->addTile(0, 0, tile[6]);
	pTMR->addTile(0, 1, tile[7]);
	pTMR->addTile(0, 2, tile[7]);
	pTMR->addTile(0, 3, tile[7]);
	pTMR->addTile(0, 4, tile[7]);
	pTMR->addTile(0, 5, tile[8]);

	for (Uint64 i = 1; i < 24; i++)
	{
		pTMR->addTile(i, 0, tile[3]);
		pTMR->addTile(i, 1, tile[4]);
		pTMR->addTile(i, 2, tile[4]);
		pTMR->addTile(i, 3, tile[4]);
		pTMR->addTile(i, 4, tile[4]);
		pTMR->addTile(i, 5, tile[5]);
	}

	pTMR->addTile(24, 0, tile[0]);
	pTMR->addTile(24, 1, tile[1]);
	pTMR->addTile(24, 2, tile[1]);
	pTMR->addTile(24, 3, tile[1]);
	pTMR->addTile(24, 4, tile[1]);
	pTMR->addTile(24, 5, tile[2]);

	//left wall ledge
	pTMR->addTile(10, 5, tile[11]);
	pTMR->addTile(11, 5, tile[4]);
	pTMR->addTile(12, 5, tile[4]);
	pTMR->addTile(13, 5, tile[4]);
	pTMR->addTile(14, 5, tile[10]);

	for (Uint64 i = 6; i <= 9; i++)
	{
		pTMR->addTile(10, i, tile[7]);
		pTMR->addTile(11, i, tile[4]);
		pTMR->addTile(12, i, tile[4]);
		pTMR->addTile(13, i, tile[4]);
		pTMR->addTile(14, i, tile[1]);
	}

	pTMR->addTile(10, 10, tile[8]);
	pTMR->addTile(11, 10, tile[5]);
	pTMR->addTile(12, 10, tile[5]);
	pTMR->addTile(13, 10, tile[5]);
	pTMR->addTile(14, 10, tile[2]);

	//floor
	pTMR->addTile(0, 6, tile[6]);
	pTMR->addTile(1, 6, tile[3]);
	pTMR->addTile(2, 6, tile[0]);

	for (Uint64 i = 7; i < 90; i++)
	{
		pTMR->addTile(0, i, tile[7]);
		pTMR->addTile(1, i, tile[4]);
		pTMR->addTile(2, i, tile[1]);
	}

	//sky platform 1
	//X = 49-51, Y = 10-14
	pTMR->addTile(10, 49, tile[6]);
	pTMR->addTile(11, 49, tile[3]);
	pTMR->addTile(12, 49, tile[3]);
	pTMR->addTile(13, 49, tile[3]);
	pTMR->addTile(14, 49, tile[0]);

	pTMR->addTile(10, 50, tile[7]);
	pTMR->addTile(11, 50, tile[4]);
	pTMR->addTile(12, 50, tile[4]);
	pTMR->addTile(13, 50, tile[4]);
	pTMR->addTile(14, 50, tile[1]);

	pTMR->addTile(10, 51, tile[8]);
	pTMR->addTile(11, 51, tile[5]);
	pTMR->addTile(12, 51, tile[5]);
	pTMR->addTile(13, 51, tile[5]);
	pTMR->addTile(14, 51, tile[2]);

	//sky platform 2
	//X = 60-82, Y = 6-13
	pTMR->addTile(6, 60, tile[6]);
	pTMR->addTile(7, 60, tile[3]);
	pTMR->addTile(8, 60, tile[3]);
	pTMR->addTile(9, 60, tile[3]);
	pTMR->addTile(10, 60, tile[3]);
	pTMR->addTile(11, 60, tile[3]);
	pTMR->addTile(12, 60, tile[3]);
	pTMR->addTile(13, 60, tile[0]);

	for (Uint64 i = 61; i < 82; i++)
	{
		pTMR->addTile(6, i, tile[7]);
		pTMR->addTile(7, i, tile[4]);
		pTMR->addTile(8, i, tile[4]);
		pTMR->addTile(9, i, tile[4]);
		pTMR->addTile(10, i, tile[4]);
		pTMR->addTile(11, i, tile[4]);
		pTMR->addTile(12, i, tile[4]);
		pTMR->addTile(13, i, tile[1]);
	}

	pTMR->addTile(6, 82, tile[8]);
	pTMR->addTile(7, 82, tile[5]);
	pTMR->addTile(8, 82, tile[5]);
	pTMR->addTile(9, 82, tile[5]);
	pTMR->addTile(10, 82, tile[5]);
	pTMR->addTile(11, 82, tile[5]);
	pTMR->addTile(12, 82, tile[5]);
	pTMR->addTile(13, 82, tile[2]);

	//platforms
	pTMR->addTile(9, 11, tile[13]);
	pTMR->addTile(9, 16, tile[13]);
	pTMR->addTile(9, 21, tile[13]);

	pTMR->addTile(9, 36, tile[13]);
	pTMR->addTile(9, 41, tile[13]);
	pTMR->addTile(9, 46, tile[13]);

	pTMR->addTile(3, 19, tile[13]);
	pTMR->addTile(4, 22, tile[13]);
	pTMR->addTile(5, 25, tile[13]);
	pTMR->addTile(7, 28, tile[13]);
	pTMR->addTile(5, 31, tile[13]);
	pTMR->addTile(4, 34, tile[13]);
	pTMR->addTile(3, 37, tile[13]);
	Platform* pPlatform = new Platform("Temp Plat", Vector2D(0,0),Vector2D(200,300), 0.f);
	pPlatform->setPos(Vector2D(0, 0));

	GameObjectManager::getInstance()->addObject(pPlatform);
}

void Level1Scene::onUnloadResources()
{
	TextureManager::getInstance()->unload("player_idle");
	TextureManager::getInstance()->unload("player_run");
	TextureManager::getInstance()->unload("player_jump");

	TextureManager::getInstance()->unload("Trees_BG");
	TextureManager::getInstance()->unload("Grass_Tile_BC");
	TextureManager::getInstance()->unload("Grass_Tile_BL");
	TextureManager::getInstance()->unload("Grass_Tile_BR");
	TextureManager::getInstance()->unload("Grass_Tile_MC");
	TextureManager::getInstance()->unload("Grass_Tile_ML");
	TextureManager::getInstance()->unload("Grass_Tile_MR");
	TextureManager::getInstance()->unload("Grass_Tile_TC");
	TextureManager::getInstance()->unload("Grass_Tile_TL");
	TextureManager::getInstance()->unload("Grass_Tile_TR");
	TextureManager::getInstance()->unload("Grass_Tile_C1");
	TextureManager::getInstance()->unload("Grass_Tile_C2");
	TextureManager::getInstance()->unload("Grass_Tile_C3");
	TextureManager::getInstance()->unload("Grass_Tile_C4");
	TextureManager::getInstance()->unload("Grass_Platform");
}
