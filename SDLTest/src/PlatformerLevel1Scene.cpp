#include "PlatformerLevel1Scene.h"
#include "TextureManager.h"
#include "GameObjectManager.h"
#include "CameraManager.h"
#include "PhysicsSystem.h"
#include "Player.h"
#include "TileMap.h"
#include "TileMapRenderer.h"
#include "AnimatedSprite.h"
#include "SpriteAnimator.h"
#include "Background.h"
#include "Platform.h"
#include "Sprite.h"
#include "Gem.h"
#include "GUIButton.h"
#include "ObjectiveManager.h"
#include "Gate.h"
#include "StoryWindow.h"
#include "AudioManager.h"
#include "InventoryGUI.h"
PlatformerLevel1Scene::PlatformerLevel1Scene() : AScene(SceneTag::PLATFORMER_LEVEL_1_SCENE)
{

}

PlatformerLevel1Scene::~PlatformerLevel1Scene()
{

}

void PlatformerLevel1Scene::addPickupDialogue(PlatformerPickupGUI* pPickupGUI, InventoryGUI* pInvenGUI)
{
	std::vector<std::string> vecGemType;
	std::vector<std::string> vecTitles;
	std::vector<std::string> vecText;
	vecGemType.push_back("Gem_Cyan");
	vecGemType.push_back("Gem_Green");
	vecGemType.push_back("Gem_Orange");
	vecGemType.push_back("Gem_Purple");
	vecGemType.push_back("Gem_Red");
	vecTitles.push_back("Character");
	vecTitles.push_back("Initiating Event");
	vecTitles.push_back("Problem");
	vecTitles.push_back("Plan");
	vecTitles.push_back("Consequence");
	vecText.push_back("The Young Moth is the Main \n Character");
	vecText.push_back("To Experience the flame's \n beauty");
	vecText.push_back("it was deadly");
	vecText.push_back("She ignored her mother's \n warning");
	vecText.push_back("She died, teaching the \n narrator to respect danger");
	for (int i = 0; i < vecGemType.size(); i++)
	{
		pPickupGUI->addPickupDialogue(vecGemType[i], vecTitles[i]);   // Title as first line
		pPickupGUI->addPickupDialogue(vecGemType[i], vecText[i]);     // Text as second line
		pInvenGUI->addPickupDialogue(vecGemType[i], vecTitles[i]);   // Title as first line
		pInvenGUI->addPickupDialogue(vecGemType[i], vecText[i]);     // Text as second line
	}
}

void PlatformerLevel1Scene::onLoadResources()
{
	TextureManager::getInstance()->loadFromFolder("animations/player_idle", "player_idle");
	TextureManager::getInstance()->loadFromFolder("animations/player_run", "player_run");
	TextureManager::getInstance()->loadFromFolder("animations/player_jump", "player_jump");
	TextureManager::getInstance()->loadFromFolder("animations/player_falling", "player_fall");
	TextureManager::getInstance()->loadFromFolder("animations/pause_animation", "Pause");
	TextureManager::getInstance()->loadFromFolder("animations/player_grab_gem", "Pickup");
	TextureManager::getInstance()->loadFromFolder("animations/inventory_animation", "Inventory");

	TextureManager::getInstance()->load("platformer/trees.png", "Trees_BG");
	TextureManager::getInstance()->load("platformer/gate.png", "Gate");
	TextureManager::getInstance()->load("left_room/statue_carabao.png", "Statue_Carabao");

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
	TextureManager::getInstance()->load("tilemaps/grass_platform.png", "Grass_Platform");

	TextureManager::getInstance()->load("gems/gem_cyan.png", "Gem_Cyan");
	TextureManager::getInstance()->load("gems/gem_green.png", "Gem_Green");
	TextureManager::getInstance()->load("gems/gem_orange.png", "Gem_Orange");
	TextureManager::getInstance()->load("gems/gem_purple.png", "Gem_Purple");
	TextureManager::getInstance()->load("gems/gem_red.png", "Gem_Red");
	TextureManager::getInstance()->load("gems/blue_grab_gem.png", "Gem_Cyan_Grab");
	TextureManager::getInstance()->load("gems/green_grab_gem.png", "Gem_Green_grab");
	TextureManager::getInstance()->load("gems/orange_grab_gem.png", "Gem_Orange_Grab");
	TextureManager::getInstance()->load("gems/purple_grab_gem.png", "Gem_Purple_grab");
	TextureManager::getInstance()->load("gems/red_grab_gem.png", "Gem_Red_Grab");
	TextureManager::getInstance()->load("gems/blue_inventory_gem.png", "Gem_Cyan_Inventory");
	TextureManager::getInstance()->load("gems/green_inventory_gem.png", "Gem_Green_Inventory");
	TextureManager::getInstance()->load("gems/orange_inventory_gem.png", "Gem_Orange_Inventory");
	TextureManager::getInstance()->load("gems/purple_inventory_gem.png", "Gem_Purple_Inventory");
	TextureManager::getInstance()->load("gems/red_inventory_gem.png", "Gem_Red_Inventory");
	TextureManager::getInstance()->load("gems/colorless_inventory_gem.png", "Gem_Colorless_Inventory");

	TextureManager::getInstance()->load("GUI/pause.png", "Pause_Button");
	TextureManager::getInstance()->load("GUI/story.png", "Story_Button");
	TextureManager::getInstance()->load("GUI/inventory.png", "Items_Button");
	TextureManager::getInstance()->load("GUI/button6.png", "Close_Button");
	TextureManager::getInstance()->load("Square.png", "Square");
	TextureManager::getInstance()->load("GUI/arrow.png", "Arrow");
	TextureManager::getInstance()->load("GUI/question_mark.png", "Q_Mark");
	TextureManager::getInstance()->load("GUI/back.png", "Return");
	TextureManager::getInstance()->load("GUI/tablet.png", "Tablet");

	//AudioManager::getInstance()->load("Audio/error.wav", "error");
	//AudioManager::getInstance()->load("Audio/TheFatRat - Unity.wav", "Unity");
	AudioManager::getInstance()->load("sounds/Music/Jungle.wav", "Jungle");
	AudioManager::getInstance()->load("sounds/SFX/arcade-game-jump.wav", "Jump");
	AudioManager::getInstance()->load("sounds/SFX/feet-landing-jump.wav", "Land");
	AudioManager::getInstance()->load("sounds/SFX/Pickup_Gem.wav", "Pickup");
}

void PlatformerLevel1Scene::onLoadObjects()
{
	CameraManager::getInstance()->getCurrentCamera()->setPos(Vector2D(1350, 700));

	PhysicsSystem::initialize();
	
	for (int i = 0; i < 3; i++)
	{
		std::string name = "Trees_BG_" + std::to_string(i);
		Sprite* pBG = new Sprite(name, "Trees_BG", Vector2D((i * 4000.0f) + 2450.0f, 1520.0f), 1.0f);
		GameObjectManager::getInstance()->addObject(pBG);
	}

	TileMap* tileMap = new TileMap("Platforms");
	GameObjectManager::getInstance()->addObject(tileMap);
	TileMapRenderer* pTMR = (TileMapRenderer*)tileMap->findComponentByName("TileMapRenderer");

	Gate* pGate = new Gate("Exit_Gate");
	pGate->setPos(Vector2D(10000.0f, 1130.0f));
	GameObjectManager::getInstance()->addObject(pGate);

	Sprite* pStatue = new Sprite("Statue_Carabao", "Statue_Carabao", Vector2D(10380.0f, 1100.0f), Vector2D(0.3f));
	GameObjectManager::getInstance()->addObject(pStatue);

	Gem* pGem1 = new Gem("Gem_Cyan");
	pGem1->setPos(Vector2D(1280, 1170));
	GameObjectManager::getInstance()->addObject(pGem1);

	Gem* pGem2 = new Gem("Gem_Green");
	pGem2->setPos(Vector2D(4860, 1170));
	GameObjectManager::getInstance()->addObject(pGem2);

	Gem* pGem3 = new Gem("Gem_Orange");
	pGem3->setPos(Vector2D(4860, 410));
	GameObjectManager::getInstance()->addObject(pGem3);

	Gem* pGem4 = new Gem("Gem_Purple");
	pGem4->setPos(Vector2D(6200, 410));
	GameObjectManager::getInstance()->addObject(pGem4);

	Gem* pGem5 = new Gem("Gem_Red");
	pGem5->setPos(Vector2D(9060, 600));
	GameObjectManager::getInstance()->addObject(pGem5);

	Player* pPlayer = new Player(Vector2D(1050, 450), Vector2D(0.6f, 0.6f), 0.f);
	GameObjectManager::getInstance()->addObject(pPlayer);

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
	unsigned int indices[] = {
		12, 9,
		17, 9,
		22, 9,
		23, 9,

		36, 9,
		37, 9,
		42, 9,
		47, 9,

		20, 3,
		23, 4,
		26, 5,
		29, 7,
		30, 7,
		33, 5,
		36, 4,
		39, 3,

		88, 4,
	};

	for (int i = 0; i < 17; i++)
	{
		int index = i * 2;
		Uint64 c = indices[index];
		Uint64 r = indices[index + 1];
		pTMR->addTile(c, r, tile[13]);
		std::string name = "Platform_";
		Platform* pPlatform = new Platform(name + std::to_string(i), pTMR->getTilePosition(c, r, true), Vector2D(300, 100), 0.f);
		GameObjectManager::getInstance()->addObject(pPlatform);
	}

	Platform* pFloor = new Platform("Floor", Vector2D(5610, 150), Vector2D(10000, 310), 0.f);
	GameObjectManager::getInstance()->addObject(pFloor);

	Platform* pFloor1 = new Platform("Floor_1", Vector2D(9318, 460), Vector2D(205, 310), 0.f);
	GameObjectManager::getInstance()->addObject(pFloor1);

	Platform* pFloor2 = new Platform("Floor_2", Vector2D(9525, 716), Vector2D(205, 205), 0.f);
	GameObjectManager::getInstance()->addObject(pFloor2);

	Platform* pFloor3 = new Platform("Floor_3", Vector2D(10140, 870), Vector2D(1020, 100), 0.f);
	GameObjectManager::getInstance()->addObject(pFloor3);

	Platform* pLeftWall = new Platform("Left_Wall", Vector2D(305,1050), Vector2D(610, 2000), 0.f);
	GameObjectManager::getInstance()->addObject(pLeftWall);

	Platform* pLeftWallLedge = new Platform("Left_Wall_Ledge", pTMR->getTilePosition(8, 12), Vector2D(520, 520), 0.f);
	GameObjectManager::getInstance()->addObject(pLeftWallLedge);

	Platform* pRightWall = new Platform("Right_Wall", Vector2D(10955,1660), Vector2D(610, 2000), 0.f);
	GameObjectManager::getInstance()->addObject(pRightWall);

	Platform* pSkyPlat1 = new Platform("Sky_Platform_1", pTMR->getTilePosition(50, 12), Vector2D(305, 510), 0.f);
	GameObjectManager::getInstance()->addObject(pSkyPlat1);

	Vector2D skyPlat2Pos = pTMR->getTilePosition(71, 9);
	skyPlat2Pos.y += 50.0f;
	Platform* pSkyPlat2 = new Platform("Sky_Platform_2", skyPlat2Pos, Vector2D(2353, 820), 0.f);
	GameObjectManager::getInstance()->addObject(pSkyPlat2);

	Platform* pSkyPlat3 = new Platform("Sky_Platform_3", pTMR->getTilePosition(97, 19), Vector2D(305, 1330), 0.f);
	GameObjectManager::getInstance()->addObject(pSkyPlat3);

	GUIButton* pPauseButton = new GUIButton("Pause_Button", "Pause_Button");
	pPauseButton->setIsScreenObject(true);
	pPauseButton->setPos(Vector2D(100.0f, 100.0f));
	pPauseButton->setScale(Vector2D(0.08f));
	GUIToggle* pPauseToggle = new GUIToggle(EventKey::PAUSE_SCREEN);
	pPauseButton->attachComponent(pPauseToggle);

	GUIButton* pStoryButton = new GUIButton("Story_Button", "Story_Button");
	pStoryButton->setIsScreenObject(true);
	pStoryButton->setPos(Vector2D(1640.0f, 100.0f));
	pStoryButton->setScale(Vector2D(0.08f));
	GUIToggle* pStoryToggle = new GUIToggle(EventKey::STORY_SCREEN);
	pStoryButton->attachComponent(pStoryToggle);

	GUIButton* pItemsButton = new GUIButton("Items_Button", "Items_Button");
	pItemsButton->setIsScreenObject(true);
	pItemsButton->setPos(Vector2D(1820.0f, 100.0f));
	pItemsButton->setScale(Vector2D(0.08f));
	GUIToggle* pItemsToggle = new GUIToggle(EventKey::INVENTORY_SCREEN);
	pItemsButton->attachComponent(pItemsToggle);

	GameObjectManager::getInstance()->addObject(pPauseButton);
	GameObjectManager::getInstance()->addObject(pStoryButton);
	GameObjectManager::getInstance()->addObject(pItemsButton);

	PauseScreen* pPauseScreen = new PauseScreen("PauseScreen");
	GameObjectManager::getInstance()->addObject(pPauseScreen);

	// ------ OBJECTIVE MANAGER ------
	// --- OBJECTIVE BUTTON creation ---
	GUIButton* pObjectiveButton = new GUIButton("Objective_Button", "Square");
	pObjectiveButton->setIsScreenObject(true);
	pObjectiveButton->setPos(Vector2D(54.0f, 365.0f));
	pObjectiveButton->setScale(Vector2D(0.08f));

	// create a Background child that actually provides a SpriteRenderer for hit tests
	Background* pObjBack = new Background("Objective_Button_Back", "Square", Vector2D(1.f));
	pObjBack->setIsScreenObject(true);
	pObjBack->setPos(Vector2D(34.0f, 345.0f)); // RELATIVE to GUIButton
	pObjBack->setScale(Vector2D(0.08f));
	pObjectiveButton->attachChild(pObjBack);

	// button settings
	SpriteRenderer* pObjSR = (SpriteRenderer*)pObjBack->findComponentByName("SpriteRenderer");
	if (pObjSR) {
		pObjSR->setColor({ 238, 202, 161, 255 });
		std::cout << "[Scene] Objective_Button SpriteRenderer found on Background; color set\n";
		std::cout << "[Scene] DEBUG: pObjSR OK, texture ptr=" << pObjSR->getTexture() << "\n";
	}
	else {
		std::cout << "[Scene] WARNING: Objective_Button SpriteRenderer NOT found on Background\n";
	}

	// Button input + toggle attached to the background (the hit target)
	ButtonInput* pBtnInput = new ButtonInput(pObjSR);
	pObjBack->attachComponent(pBtnInput);
	GUIToggle* pObjectiveToggle = new GUIToggle(EventKey::OBJECTIVE_BUTTON);
	pObjBack->attachComponent(pObjectiveToggle);

	// register it so it draws and receives input
	GameObjectManager::getInstance()->addObject(pObjectiveButton);
	
	// --- ARROW creation ---
	Sprite* pArrow = new Sprite("Arrow", "Arrow", Vector2D(1077.777f, 463.686f), 1.0f);
	pArrow->setScale(Vector2D(.8f));
	pArrow->setIsScreenObject(false);                   // optional: treat as UI so it doesn't move with camera
	pArrow->setEnabled(false);                          // start hidden
	GameObjectManager::getInstance()->addObject(pArrow);

	// create objectivemanager object
	Sprite* pObjectiveManagerObj = new Sprite("ObjectiveManager_Object", "Arrow", Vector2D(0, 0), 1.0f);
	pObjectiveManagerObj->setEnabled(true);
	ObjectiveManager* pObjectiveManager = new ObjectiveManager();
	pObjectiveManagerObj->attachComponent(pObjectiveManager);
	GameObjectManager::getInstance()->addObject(pObjectiveManagerObj);

	StoryWindow* pStoryWindow = new StoryWindow("Platformer1StoryWindow");
	pStoryWindow->setPos(Vector2D(0, 0));
	GameObjectManager::getInstance()->addObject(pStoryWindow);
	pStoryWindow->setEnabled(false);
	pStoryWindow->setPos(Vector2D(0, 0));

	PlatformerPickupGUI* pPickupGUI = new PlatformerPickupGUI("Platformer1PickupGUI");
	GameObjectManager::getInstance()->addObject(pPickupGUI);

	InventoryGUI* pInventoryGUI = new InventoryGUI("Platformer1InventoryGUI");
	GameObjectManager::getInstance()->addObject(pInventoryGUI);
	this->addPickupDialogue(pPickupGUI,pInventoryGUI);

	AudioManager::getInstance()->play(new AudioPlayer("Jungle", "BGM", AudioGroupTag::MUSIC, OnAudioFinished::LOOP));
}

void PlatformerLevel1Scene::onUnloadResources()
{
	TextureManager::getInstance()->unload("player_idle");
	TextureManager::getInstance()->unload("player_run");
	TextureManager::getInstance()->unload("player_jump");
	TextureManager::getInstance()->unload("player_fall");
	TextureManager::getInstance()->unload("Pause");
	TextureManager::getInstance()->unload("Pickup");
	TextureManager::getInstance()->unload("Inventory");

	TextureManager::getInstance()->unload("Trees_BG");
	TextureManager::getInstance()->unload("Gate");

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

	TextureManager::getInstance()->unload("Gem_Cyan");
	TextureManager::getInstance()->unload("Gem_Cyan_Grab");
	TextureManager::getInstance()->unload("Gem_Cyan_Inventory");
	TextureManager::getInstance()->unload("Gem_Green");
	TextureManager::getInstance()->unload("Gem_Green_Grab");
	TextureManager::getInstance()->unload("Gem_Green_Inventory");
	TextureManager::getInstance()->unload("Gem_Orange");
	TextureManager::getInstance()->unload("Gem_Orange_Grab");
	TextureManager::getInstance()->unload("Gem_Orange_Inventory");
	TextureManager::getInstance()->unload("Gem_Purple");
	TextureManager::getInstance()->unload("Gem_Purple_Grab");
	TextureManager::getInstance()->unload("Gem_Purple_Inventory");
	TextureManager::getInstance()->unload("Gem_Red");
	TextureManager::getInstance()->unload("Gem_Red_Grab");
	TextureManager::getInstance()->unload("Gem_Red_Inventory");
	TextureManager::getInstance()->unload("Gem_Colorless_Inventory");



	TextureManager::getInstance()->unload("Pause_Button");
	TextureManager::getInstance()->unload("Story_Button");
	TextureManager::getInstance()->unload("Items_Button");
	TextureManager::getInstance()->unload("Close_Button");
	TextureManager::getInstance()->unload("Square");
	TextureManager::getInstance()->unload("Arrow");
	TextureManager::getInstance()->unload("Q_Mark");
	TextureManager::getInstance()->unload("Tablet");

	//AudioManager::getInstance()->unload("error");
	//AudioManager::getInstance()->unload("Unity");
	AudioManager::getInstance()->unload("Jungle");
	AudioManager::getInstance()->unload("Jump");
	AudioManager::getInstance()->unload("Land");
	AudioManager::getInstance()->unload("Pickup");
}

void PlatformerLevel1Scene::onUnloadObjects()
{
	AudioManager::getInstance()->stopAll();
	AScene::onUnloadObjects();
}
