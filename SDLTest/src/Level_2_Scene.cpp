#include "Level_2_Scene.h"
#include "FontManager.h"
#include "TrackerManager.h"
#include "GemManager.h"
#include "GUIButton.h"
#include "SceneSwitcher.h"
#include "TurnCounter.h"
#include "EndScreen.h"
#include "BubbleManager.h"
#include "GUIUtils.h"

Level_2_Scene::Level_2_Scene():AScene(SceneTag::LEVEL_2_SCENE)
{
}

void Level_2_Scene::onLoadResources()
{
	TextureManager::getInstance()->load("levels/Level_2.png", "Level_Container");
	FontManager::getInstance()->loadFont("CurseCasual.ttf", "Curse45", 45);
	GemManager::loadResources();
}

void Level_2_Scene::onLoadObjects()
{
	this->loadEmptyObjects();

	Background* pBackground = new Background("Level_Background", "Level_Background", Vector2D(125.f, 1.17f));
	GameObjectManager::getInstance()->addObject((AGameObject*)pBackground);
	BubbleManager::initialize(100);

	Sprite* pObjectBG = new Sprite("Design_BG", "Design_BG", Vector2D(0, -330), Vector2D(2, 2), 0.0f, false);
	GameObjectManager::getInstance()->addObject((AGameObject*)pObjectBG);

	Sprite* pLevelContainer = new Sprite("Level_Container", "Level_Container", Vector2D(0, 0), Vector2D(2, 2), 0.0f, false);
	GUIUtils::setGUIMidCenter(pLevelContainer, Vector2D(9, 40));
	GameObjectManager::getInstance()->addObject((AGameObject*)pLevelContainer);

	this->loadGUI();

	GemManager::initialize(10, 8, 60.0f, 0.2f, Vector2D(30.0f, -72.0f));

	std::vector<Uint8> r0Cols = { 4, 5 };
	std::vector<Uint8> r0r6Cols = { 0, 1, 8, 9 };
	std::vector<Uint8> r1r5Cols = { 0, 9 };
	std::vector<Uint8> r7Cols = { 0, 1, 2, 7, 8, 9 };

	GemManager::getInstance()->setBlocked(0, r0Cols, true);
	GemManager::getInstance()->setBlocked(0, r0r6Cols, true);
	GemManager::getInstance()->setBlocked(1, r1r5Cols, true);
	GemManager::getInstance()->setBlocked(5, r1r5Cols, true);
	GemManager::getInstance()->setBlocked(6, r0r6Cols, true);
	GemManager::getInstance()->setBlocked(7, r7Cols, true);

	GemManager::getInstance()->setSpawnBombsAuto(true);
	GemManager::getInstance()->placeGem(GemType::CRATE_2, 4, 1);
	GemManager::getInstance()->placeGem(GemType::CRATE_2, 5, 2);
	GemManager::getInstance()->placeGem(GemType::CRATE_2, 4, 3);
	GemManager::getInstance()->placeGem(GemType::CRATE_2, 4, 6);
	GemManager::getInstance()->placeGem(GemType::CRATE_2, 5, 7);
	GemManager::getInstance()->placeGem(GemType::CRATE_2, 4, 8);
	GemManager::getInstance()->spawnGems();
}

void Level_2_Scene::onUnloadResources()
{
	GemManager::unloadResources();
}

void Level_2_Scene::onUnloadObjects()
{
	TrackerManager::destroy();
	AScene::onUnloadObjects();
}
void Level_2_Scene::loadEmptyObjects()
{
	EmptyObject* pTrackerManagerHolder = new EmptyObject("TrackerManagerHolder");
	TrackerManager::initialize("TrackerManager", pTrackerManagerHolder);
	GameObjectManager::getInstance()->addObject(pTrackerManagerHolder);
}
void Level_2_Scene::loadGUI()
{
	GemManager::loadGUI();

	Tracker* pRedTracker = new Tracker("Red_Tracker", "Red", GemType::RED, 8);
	GameObjectManager::getInstance()->addObject(pRedTracker);
	GUIUtils::setGUITopLeft(pRedTracker, Vector2D(110, 25));
	pRedTracker->setScale(Vector2D(.15, .15));
	TrackerManager::getInstance()->registerTracker(pRedTracker);

	Tracker* pBlueTracker = new Tracker("Blue_Tracker", "Blue", GemType::BLUE, 8);
	GameObjectManager::getInstance()->addObject(pBlueTracker);
	GUIUtils::setGUITopLeft(pBlueTracker, Vector2D(20, 25));
	pBlueTracker->setScale(Vector2D(.15, .15));
	TrackerManager::getInstance()->registerTracker(pBlueTracker);

	Tracker* pPurpleTracker = new Tracker("Purple_Tracker", "Purple", GemType::PURPLE, 8);
	GameObjectManager::getInstance()->addObject(pPurpleTracker);
	GUIUtils::setGUITopLeft(pPurpleTracker, Vector2D(110, 75));
	pPurpleTracker->setScale(Vector2D(.15, .15));
	TrackerManager::getInstance()->registerTracker(pPurpleTracker);

	Tracker* pWhiteTracker = new Tracker("White_Tracker", "White", GemType::WHITE, 8);
	GameObjectManager::getInstance()->addObject(pWhiteTracker);
	GUIUtils::setGUITopLeft(pWhiteTracker, Vector2D(20, 75));
	pWhiteTracker->setScale(Vector2D(.15, .15));
	TrackerManager::getInstance()->registerTracker(pWhiteTracker);

	EndScreen* pEndScreen = new EndScreen("EndScreen");
	pEndScreen->setEnabled(false);
	GameObjectManager::getInstance()->addObject(pEndScreen);
}