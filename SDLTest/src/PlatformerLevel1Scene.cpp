#include "PlatformerLevel1Scene.h"
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
PlatformerLevel1Scene::PlatformerLevel1Scene() : AScene(SceneTag::PLATFORMER_LEVEL_1_SCENE)
{

}

PlatformerLevel1Scene::~PlatformerLevel1Scene()
{

}

void PlatformerLevel1Scene::onLoadResources()
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

void PlatformerLevel1Scene::onLoadObjects()
{
	CameraManager::getInstance()->getCurrentCamera()->setPos(Vector2D(1350, 700));

	PhysicsManager::initialize();

	//Player* pPlayer = new Player(Vector2D(200, 200), Vector2D(), 0.0f);
	//GameObjectManager::getInstance()->addObject(pPlayer);
	
	Background* pBG1 = new Background("Trees_BG1", "Trees_BG", 0.36f);
	pBG1->setPos(Vector2D(0.0f, 0.0f));
	GameObjectManager::getInstance()->addObject(pBG1);
	
	Background* pBG2 = new Background("Trees_BG2", "Trees_BG", 0.36f);
	pBG2->setPos(Vector2D(1440.0f, 0.0f));
	GameObjectManager::getInstance()->addObject(pBG2);

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
	pTMR->addTile(1, 0, tile[7]);
	pTMR->addTile(2, 0, tile[7]);
	pTMR->addTile(3, 0, tile[7]);
	pTMR->addTile(4, 0, tile[7]);
	pTMR->addTile(5, 0, tile[8]);

	for (Uint64 i = 1; i < 25; i++)
	{
		pTMR->addTile(0, i, tile[3]);
		pTMR->addTile(1, i, tile[4]);
		pTMR->addTile(2, i, tile[4]);
		pTMR->addTile(3, i, tile[4]);
		pTMR->addTile(4, i, tile[4]);
		pTMR->addTile(5, i, tile[5]);
	}

	pTMR->addTile(0, 25, tile[0]);
	pTMR->addTile(1, 25, tile[1]);
	pTMR->addTile(2, 25, tile[1]);
	pTMR->addTile(3, 25, tile[1]);
	pTMR->addTile(4, 25, tile[1]);
	pTMR->addTile(5, 25, tile[2]);

	//left wall ledge
	pTMR->addTile(5, 10, tile[11]);
	pTMR->addTile(5, 11, tile[4]);
	pTMR->addTile(5, 12, tile[4]);
	pTMR->addTile(5, 13, tile[4]);
	pTMR->addTile(5, 14, tile[10]);

	for (Uint64 i = 6; i <= 9; i++)
	{
		pTMR->addTile(i, 10, tile[7]);
		pTMR->addTile(i, 11, tile[4]);
		pTMR->addTile(i, 12, tile[4]);
		pTMR->addTile(i, 13, tile[4]);
		pTMR->addTile(i, 14, tile[1]);
	}

	pTMR->addTile(10, 10, tile[8]);
	pTMR->addTile(10, 11, tile[5]);
	pTMR->addTile(10, 12, tile[5]);
	pTMR->addTile(10, 13, tile[5]);
	pTMR->addTile(10, 14, tile[2]);

	//floor
	pTMR->addTile(6, 0, tile[6]);
	pTMR->addTile(6, 1, tile[3]);
	pTMR->addTile(6, 2, tile[0]);

	for (Uint64 i = 7; i < 90; i++)
	{
		pTMR->addTile(i, 0, tile[7]);
		pTMR->addTile(i, 1, tile[4]);
		pTMR->addTile(i, 2, tile[1]);
	}

	for (Uint64 i = 90; i < 104; i++)
	{
		pTMR->addTile(i, 0, tile[7]);
		pTMR->addTile(i, 1, tile[4]);
		pTMR->addTile(i, 2, tile[4]);
	}

	pTMR->addTile(90, 2, tile[9]);
	pTMR->addTile(90, 3, tile[3]);
	pTMR->addTile(90, 4, tile[3]);
	pTMR->addTile(90, 5, tile[0]);
	pTMR->addTile(91, 3, tile[4]);
	pTMR->addTile(91, 4, tile[4]);
	pTMR->addTile(91, 5, tile[1]);

	for (Uint64 i = 92; i < 95; i++)
	{
		for (Uint64 j = 3; j < 8; j++)
		{
			pTMR->addTile(i, j, tile[4]);
		}
	}

	pTMR->addTile(92, 5, tile[9]);
	pTMR->addTile(92, 6, tile[3]);
	pTMR->addTile(92, 7, tile[0]);
	pTMR->addTile(93, 5, tile[4]);
	pTMR->addTile(93, 6, tile[4]);
	pTMR->addTile(93, 7, tile[1]);
	pTMR->addTile(94, 7, tile[9]);
	pTMR->addTile(94, 8, tile[0]);

	//right wall
	for (Uint64 i = 95; i < 105; i++)
	{
		pTMR->addTile(i, 3, tile[4]);
		pTMR->addTile(i, 4, tile[4]);
		pTMR->addTile(i, 5, tile[4]);
		pTMR->addTile(i, 6, tile[4]);
		pTMR->addTile(i, 7, tile[4]);
		pTMR->addTile(i, 8, tile[1]);
	}
	pTMR->addTile(104, 8, tile[9]);

	// First bottom corner of floor
	pTMR->addTile(104, 0, tile[8]);
	pTMR->addTile(104, 1, tile[5]);

	pTMR->addTile(104, 2, tile[11]);
	pTMR->addTile(105, 2, tile[7]);
	pTMR->addTile(106, 2, tile[7]);
	pTMR->addTile(107, 2, tile[7]);
	pTMR->addTile(108, 2, tile[7]);
	pTMR->addTile(109, 2, tile[8]);

	for (Uint64 i = 3; i < 25; i++)
	{
		if (i > 8)
			pTMR->addTile(104, i, tile[3]);

		pTMR->addTile(105, i, tile[4]);
		pTMR->addTile(106, i, tile[4]);
		pTMR->addTile(107, i, tile[4]);
		pTMR->addTile(108, i, tile[4]);
		pTMR->addTile(109, i, tile[5]);
	}

	pTMR->addTile(104, 25, tile[0]);
	pTMR->addTile(105, 25, tile[1]);
	pTMR->addTile(106, 25, tile[1]);
	pTMR->addTile(107, 25, tile[1]);
	pTMR->addTile(108, 25, tile[1]);
	pTMR->addTile(109, 25, tile[2]);

	//sky platform 1
	//X = 49-51, Y = 10-14
	pTMR->addTile(49, 10, tile[6]);
	pTMR->addTile(50, 10, tile[7]);
	pTMR->addTile(51, 10, tile[8]);

	for (Uint64 i = 11; i < 14; i++)
	{
		pTMR->addTile(49, i, tile[3]);
		pTMR->addTile(50, i, tile[4]);
		pTMR->addTile(51, i, tile[5]);
	}

	pTMR->addTile(49, 14, tile[0]);
	pTMR->addTile(50, 14, tile[1]);
	pTMR->addTile(51, 14, tile[2]);

	//sky platform 2
	//X = 60-82, Y = 6-13
	pTMR->addTile(60, 6, tile[6]);
	for (Uint64 i = 7; i < 13; i++)
		pTMR->addTile(60, i, tile[3]);
	pTMR->addTile(60, 13, tile[0]);

	for (Uint64 i = 61; i < 82; i++)
	{
		pTMR->addTile(i, 6, tile[7]);
		for (Uint64 j = 7; j < 13; j++)
			pTMR->addTile(i, j, tile[4]);
		pTMR->addTile(i, 13, tile[1]);
	}

	pTMR->addTile(82, 6, tile[8]);
	for (Uint64 i = 7; i < 13; i++)
		pTMR->addTile(82, i, tile[5]);
	pTMR->addTile(82, 13, tile[2]);

	//sky platform 3
	//X = 96-98, Y = 13-25
	pTMR->addTile(96, 13, tile[6]);
	pTMR->addTile(97, 13, tile[7]);
	pTMR->addTile(98, 13, tile[8]);

	for (Uint64 i = 14; i < 25; i++)
	{
		pTMR->addTile(96, i, tile[3]);
		pTMR->addTile(97, i, tile[4]);
		pTMR->addTile(98, i, tile[5]);
	}

	pTMR->addTile(96, 25, tile[0]);
	pTMR->addTile(97, 25, tile[1]);
	pTMR->addTile(98, 25, tile[2]);

	//platforms
	pTMR->addTile(11, 9, tile[13]);
	pTMR->addTile(16, 9, tile[13]);
	pTMR->addTile(21, 9, tile[13]);

	pTMR->addTile(36, 9, tile[13]);
	pTMR->addTile(41, 9, tile[13]);
	pTMR->addTile(46, 9, tile[13]);

	pTMR->addTile(19, 3, tile[13]);
	pTMR->addTile(22, 4, tile[13]);
	pTMR->addTile(25, 5, tile[13]);
	pTMR->addTile(28, 7, tile[13]);
	pTMR->addTile(32, 5, tile[13]);
	pTMR->addTile(35, 4, tile[13]);
	pTMR->addTile(38, 3, tile[13]);

	pTMR->addTile(87, 4, tile[13]);

	Platform* pPlatform = new Platform("Temp Plat", Vector2D(0,0),Vector2D(200,300), 0.f);
	pPlatform->setPos(Vector2D(0, 0));

	GameObjectManager::getInstance()->addObject(pPlatform);
}

void PlatformerLevel1Scene::onUnloadResources()
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
