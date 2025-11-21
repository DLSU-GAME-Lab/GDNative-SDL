#include "Level_1_Scene.h"
#include "FontManager.h"
#include "GemManager.h"
#include "BubbleManager.h"
#include "TrackerManager.h"
#include "GUIButton.h"
#include "SceneSwitcher.h"
#include "Tracker.h"
#include "TurnCounter.h"
#include "EndScreen.h"
Level_1_Scene::Level_1_Scene():AScene(SceneTag::LEVEL_1_SCENE)
{
}

void Level_1_Scene::onLoadResources()
{
	TextureManager::getInstance()->load("title_screen_pngs/Sprite_bg.png", "Level_Background");
	TextureManager::getInstance()->load("title_screen_pngs/Sprite_Window.png", "UI_Container");
	TextureManager::getInstance()->load("Menu/Sprite_UI_TopBar_BG_2.png", "Top_UI_Container");
	TextureManager::getInstance()->load("Menu/Sprite_UI_setupicon.png", "Settings");
	TextureManager::getInstance()->load("Menu/Sprite_UI_TopBar_BG_round.png", "Level_Container_Extra");
	TextureManager::getInstance()->load("Menu/Sprite_ribbon_victory.png", "Victory_Ribbon");
	TextureManager::getInstance()->load("Menu/Sprite_ribbon_defeat.png", "Defeat_Ribbon");
	TextureManager::getInstance()->load("Menu/Sprite_Button_green.png", "Green_Button");
	TextureManager::getInstance()->load("title_screen_pngs/Background_Objects.png", "Design_BG");
	TextureManager::getInstance()->load("levels/Level_1.png", "Level_Container");
	TextureManager::getInstance()->load("Sprite_Bubble_Small.png", "Bubble");
	FontManager::getInstance()->loadFont("", "Curse45", 45);
	GemManager::loadResources();
}

void Level_1_Scene::onLoadObjects()
{
	this->loadEmptyObjects();

	Background* pBackground = new Background("Level_Background", "Level_Background", Vector2D(125.f, 1.17f));
	GameObjectManager::getInstance()->addObject((AGameObject*)pBackground);

	BubbleManager::initialize(30);

	this->loadGUI();

	GemManager::initialize(9, 10, 60.0f, Vector2D(30.0f, -54.0f));
	std::vector<Uint8> r0Cols = { 0, 8 };
	std::vector<Uint8> r9Cols = { 3, 4, 5 };

	GemManager::getInstance()->setBlocked(0, r0Cols, true);
	GemManager::getInstance()->setBlocked(9, r9Cols, true);

	GemManager::getInstance()->spawnGems(0.2f);


}

void Level_1_Scene::onUnloadResources()
{
	TextureManager::getInstance()->unload("Level_Background");
	TextureManager::getInstance()->unload("Lower_UI_Container");
	TextureManager::getInstance()->unload("Settings");
	TextureManager::getInstance()->unload("Design_BG");
	TextureManager::getInstance()->unload("Level_Container");
	TextureManager::getInstance()->unload("Level_Container_Extra");
	TextureManager::getInstance()->unload("Top_UI_Container");
	TextureManager::getInstance()->unload("Victory_Ribbon");
	TextureManager::getInstance()->unload("Defeat_Ribbon");
	TextureManager::getInstance()->unload("Green_Button");
	TextureManager::getInstance()->unload("Red");
	TextureManager::getInstance()->unload("Green");
	TextureManager::getInstance()->unload("Blue");
	TextureManager::getInstance()->unload("Purple");
	TextureManager::getInstance()->unload("Yellow");
	TextureManager::getInstance()->unload("White");
	TextureManager::getInstance()->unload("Bubble");
	GemManager::unloadResources();
}

void Level_1_Scene::onUnloadObjects()
{
	TrackerManager::destroy();
	AScene::onUnloadObjects();

}
void Level_1_Scene::loadEmptyObjects()
{
	EmptyObject* pTrackerManagerHolder = new EmptyObject("TrackerManagerHolder");
	TrackerManager::initialize("TrackerManager", pTrackerManagerHolder);
	GameObjectManager::getInstance()->addObject(pTrackerManagerHolder);
}

void Level_1_Scene::loadGUI()
{
	Sprite* pObjectBG = new Sprite("Design_BG", "Design_BG", Vector2D(0, -330), Vector2D(2, 2), 0.0f, false);
	GameObjectManager::getInstance()->addObject((AGameObject*)pObjectBG);

	Sprite* pLevelContainer = new Sprite("Level_Container", "Level_Container", Vector2D(0, -20), Vector2D(2, 2), 0.0f, false);
	GameObjectManager::getInstance()->addObject((AGameObject*)pLevelContainer);

	Sprite* pLowerUIContainer = new Sprite("Lower_UI_Container", "UI_Container", Vector2D(0, -540), Vector2D(20, 2.f), 0.0f, false);
	GameObjectManager::getInstance()->addObject((AGameObject*)pLowerUIContainer);

	Sprite* pUpperUIContainer = new Sprite("Upper_UI_Container", "Top_UI_Container", Vector2D(0, 520), Vector2D(500, 1), 0.0f, false);
	GameObjectManager::getInstance()->addObject((AGameObject*)pUpperUIContainer);

	Sprite* pUIContainerExtra = new Sprite("Level_Container_Extra", "Level_Container_Extra", Vector2D(0, 547), Vector2D(1, 1), 0.0f, false);
	GameObjectManager::getInstance()->addObject((AGameObject*)pUIContainerExtra);

	GUIButton* pSettings = new GUIButton("Settings", "Settings");
	pSettings->setPos(Vector2D(910, -490));
	pSettings->setScale(Vector2D(.75f, .75f));
	GameObjectManager::getInstance()->addObject((AGameObject*)pSettings);
	SceneSwitcher* pTitleSwitcher = new SceneSwitcher(SceneTag::TITLE_SCENE);
	pSettings->attachComponent(pTitleSwitcher);

	Tracker* pRedTracker = new Tracker("Red_Tracker", "Red", GemType::RED, 8);
	GameObjectManager::getInstance()->addObject(pRedTracker);
	pRedTracker->setPos(Vector2D(-300, 500));
	pRedTracker->setScale(Vector2D(.15, .15));
	TrackerManager::getInstance()->registerTracker(pRedTracker);

	Tracker* pBlueTracker = new Tracker("Blue_Tracker", "Blue", GemType::BLUE, 8);
	GameObjectManager::getInstance()->addObject(pBlueTracker);
	pBlueTracker->setPos(Vector2D(-400, 500));
	pBlueTracker->setScale(Vector2D(.15, .15));
	TrackerManager::getInstance()->registerTracker(pBlueTracker);

	Tracker* pPurpleTracker = new Tracker("Purple_Tracker", "Purple", GemType::PURPLE, 8);
	GameObjectManager::getInstance()->addObject(pPurpleTracker);
	pPurpleTracker->setPos(Vector2D(-300, 450));
	pPurpleTracker->setScale(Vector2D(.15, .15));
	TrackerManager::getInstance()->registerTracker(pPurpleTracker);

	Tracker* pWhiteTracker = new Tracker("White_Tracker", "White", GemType::WHITE, 8);
	GameObjectManager::getInstance()->addObject(pWhiteTracker);
	pWhiteTracker->setPos(Vector2D(-400, 450));
	pWhiteTracker->setScale(Vector2D(.15, .15));
	TrackerManager::getInstance()->registerTracker(pWhiteTracker);

	TurnCounter* pTurnCount = new TurnCounter("TurnCounter", 20, Vector2D(400, 475), Vector2D(1, 1));
	GameObjectManager::getInstance()->addObject(pTurnCount);

	EndScreen* pEndScreen = new EndScreen("EndScreen");
	pEndScreen->setEnabled(false);
	GameObjectManager::getInstance()->addObject(pEndScreen);

}