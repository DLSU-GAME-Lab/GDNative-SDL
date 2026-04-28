#include "Level_3_Scene.h"
#include "FontManager.h"
#include "TrackerManager.h"
#include "GemManager.h"
#include "GUIButton.h"
#include "SceneSwitcher.h"
#include "TurnCounter.h"
#include "EndScreen.h"
#include "BubbleManager.h"
#include "GUIUtils.h"

Level_3_Scene::Level_3_Scene():AScene(SceneTag::LEVEL_3_SCENE)
{
}

void Level_3_Scene::onLoadResources()
{
	TextureManager::getInstance()->load("levels/Level_3.png", "Level_Container");
	FontManager::getInstance()->loadFont("CurseCasual.ttf", "Curse45", 45);
	GemManager::loadResources();
}

void Level_3_Scene::onLoadObjects()
{ 
	this->loadEmptyObjects();

	Background* pBackground = new Background("Level_Background", "Level_Background", Vector2D(125.f, 1.17f));
	GameObjectManager::getInstance()->addObject((AGameObject*)pBackground);
	BubbleManager::initialize(100);

	Sprite* pObjectBG = new Sprite("Design_BG", "Design_BG", Vector2D(0, -330), Vector2D(2, 2), 0.0f, false);
	GameObjectManager::getInstance()->addObject((AGameObject*)pObjectBG);
	
	Sprite* pLevelContainer = new Sprite("Level_Container", "Level_Container", Vector2D(0, 0), Vector2D(1.5f, 1.5f), 0.0f, false);
	GUIUtils::setGUIMidCenter(pLevelContainer, Vector2D(0, 50));
	GameObjectManager::getInstance()->addObject((AGameObject*)pLevelContainer);

	this->loadGUI();

	GemManager::initialize(12, 14, 45.0f, 0.15f, Vector2D(23.0f, -76.0f));

	std::vector< std::vector<Uint8>> rowBlocks;

	rowBlocks.push_back({ 0, 1, 2, 5, 6, 9, 10, 11 });			// r0
	rowBlocks.push_back({ 0, 5, 6, 11 });						// r1
	rowBlocks.push_back({ 3, 4, 5, 6, 7, 8 });					// r2
	rowBlocks.push_back({ 2, 3, 4, 5, 6, 7, 8, 9 });			// r3
	rowBlocks.push_back({ });									// r4
	rowBlocks.push_back({ });									// r5
	rowBlocks.push_back({ 2, 3, 8, 9 });						// r6
	rowBlocks.push_back({ 2, 3, 4, 7, 8, 9 });					// r7
	rowBlocks.push_back({ 1, 2, 3, 4, 7, 8, 9, 10 });			// r8
	rowBlocks.push_back({ 0, 2, 3, 4, 7, 8, 9, 11 });			// r9
	rowBlocks.push_back({ 4, 7 });								// r10
	rowBlocks.push_back({ 0, 11 });								// r11
	rowBlocks.push_back({ 0, 1, 2, 9, 10, 11 });				// r12
	rowBlocks.push_back({ 0, 1, 2, 3, 4, 7, 8, 9, 10, 11 });	// r13

	for (int i = 0; i < rowBlocks.size(); i++)
	{
		GemManager::getInstance()->setBlocked(i, rowBlocks[i], true);
	}

	GemManager::getInstance()->setSpawnBombsAuto(true);
	GemManager::getInstance()->spawnGems();
}

void Level_3_Scene::onUnloadResources()
{
	GemManager::unloadResources();
	FontManager::getInstance()->unloadAllFonts();
}

void Level_3_Scene::onUnloadObjects()
{
	TrackerManager::destroy();
	AScene::onUnloadObjects();
}
void Level_3_Scene::loadEmptyObjects()
{
	EmptyObject* pTrackerManagerHolder = new EmptyObject("TrackerManagerHolder");
	TrackerManager::initialize("TrackerManager", pTrackerManagerHolder);
	GameObjectManager::getInstance()->addObject(pTrackerManagerHolder);
}
void Level_3_Scene::loadGUI()
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