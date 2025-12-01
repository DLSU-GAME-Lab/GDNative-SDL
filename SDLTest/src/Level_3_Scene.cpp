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
	TextureManager::getInstance()->load("title_screen_pngs/Sprite_bg.png", "Level_Background");
	TextureManager::getInstance()->load("title_screen_pngs/Sprite_Window.png", "UI_Container");
	TextureManager::getInstance()->load("Menu/Sprite_UI_setupicon.png", "Settings");
	TextureManager::getInstance()->load("Menu/Sprite_ribbon_victory.png", "Victory_Ribbon");
	TextureManager::getInstance()->load("Menu/Sprite_ribbon_defeat.png", "Defeat_Ribbon");
	TextureManager::getInstance()->load("Menu/Sprite_Button_green.png", "Green_Button");
	TextureManager::getInstance()->load("title_screen_pngs/Background_Objects.png", "Design_BG");
	TextureManager::getInstance()->load("levels/Level_3.png", "Level_Container");
	TextureManager::getInstance()->load("Menu/Sprite_UI_TopBar_BG_2.png", "Top_UI_Container");
	TextureManager::getInstance()->load("Menu/Sprite_UI_TopBar_BG_round.png", "Level_Container_Extra");
	TextureManager::getInstance()->load("Sprite_Bubble_Small.png", "Bubble");
	FontManager::getInstance()->loadFont("CurseCasual.ttf", "Curse45", 45);
	GemManager::loadResources();
}

void Level_3_Scene::onLoadObjects()
{ 
	this->loadEmptyObjects();

	this->loadGUI();

	GemManager::initialize(12, 14, 45.0f, Vector2D(23.0f, -46.0f));

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

	GemManager::getInstance()->spawnGems(0.15f);
}

void Level_3_Scene::onUnloadResources()
{
	TextureManager::getInstance()->unload("Level_Background");
	TextureManager::getInstance()->unload("UI_Container");
	TextureManager::getInstance()->unload("Settings");
	TextureManager::getInstance()->unload("Design_BG");
	TextureManager::getInstance()->unload("Level_Container");
	TextureManager::getInstance()->unload("Top_UI_Container");
	TextureManager::getInstance()->unload("Level_Container_Extra");
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
	Background* pBackground = new Background("Level_Background", "Level_Background", Vector2D(125.f, 1.17f));
	GameObjectManager::getInstance()->addObject((AGameObject*)pBackground);
	BubbleManager::initialize(30);

	Sprite* pObjectBG = new Sprite("Design_BG", "Design_BG", Vector2D(0, -330), Vector2D(2, 2), 0.0f, false);
	GameObjectManager::getInstance()->addObject((AGameObject*)pObjectBG);

	Sprite* pLevelContainer = new Sprite("Level_Container", "Level_Container", Vector2D(0, 0), Vector2D(1.5f, 1.5f), 0.0f, false);
	GUIUtils::setGUIMidCenter(pLevelContainer, Vector2D(0, 20));
	GameObjectManager::getInstance()->addObject((AGameObject*)pLevelContainer);

	Sprite* pLowerUIContainer = new Sprite("Lower_UI_Container", "UI_Container", Vector2D(0, 0), Vector2D(10, 2), 0.0f, false);
	GUIUtils::setGUIBotCenter(pLowerUIContainer, Vector2D(0));
	GameObjectManager::getInstance()->addObject((AGameObject*)pLowerUIContainer);

	Sprite* pUpperUIContainer = new Sprite("Upper_UI_Container", "Top_UI_Container", Vector2D(0, 0), Vector2D(100, 1.25f), 0.0f, false);
	GUIUtils::setGUITopCenter(pUpperUIContainer, Vector2D(0));
	GameObjectManager::getInstance()->addObject((AGameObject*)pUpperUIContainer);

	Sprite* pUIContainerExtra = new Sprite("Level_Container_Extra", "Level_Container_Extra", Vector2D(0, 0), Vector2D(1, 1), 0.0f, false);
	GUIUtils::setGUITopCenter(pUIContainerExtra, Vector2D(0));
	GameObjectManager::getInstance()->addObject((AGameObject*)pUIContainerExtra);

	GUIButton* pSettings = new GUIButton("Settings", "Settings");
	GUIUtils::setGUIBotRight(pSettings, Vector2D(-60));
	pSettings->setScale(Vector2D(.75f, .75f));
	GameObjectManager::getInstance()->addObject((AGameObject*)pSettings);
	SceneSwitcher* pTitleSwitcher = new SceneSwitcher(SceneTag::TITLE_SCENE);
	pSettings->attachComponent(pTitleSwitcher);

	Tracker* pRedTracker = new Tracker("Red_Tracker", "Red", GemType::RED, 8);
	GameObjectManager::getInstance()->addObject(pRedTracker);
	GUIUtils::setGUITopLeft(pRedTracker, Vector2D(104, 4));
	pRedTracker->setScale(Vector2D(.15, .15));
	TrackerManager::getInstance()->registerTracker(pRedTracker);

	Tracker* pBlueTracker = new Tracker("Blue_Tracker", "Blue", GemType::BLUE, 8);
	GameObjectManager::getInstance()->addObject(pBlueTracker);
	GUIUtils::setGUITopLeft(pBlueTracker, Vector2D(4, 4));
	pBlueTracker->setScale(Vector2D(.15, .15));
	TrackerManager::getInstance()->registerTracker(pBlueTracker);

	Tracker* pPurpleTracker = new Tracker("Purple_Tracker", "Purple", GemType::PURPLE, 8);
	GameObjectManager::getInstance()->addObject(pPurpleTracker);
	GUIUtils::setGUITopLeft(pPurpleTracker, Vector2D(104, 54));
	pPurpleTracker->setScale(Vector2D(.15, .15));
	TrackerManager::getInstance()->registerTracker(pPurpleTracker);

	Tracker* pWhiteTracker = new Tracker("White_Tracker", "White", GemType::WHITE, 8);
	GameObjectManager::getInstance()->addObject(pWhiteTracker);
	GUIUtils::setGUITopLeft(pWhiteTracker, Vector2D(4, 54));
	pWhiteTracker->setScale(Vector2D(.15, .15));
	TrackerManager::getInstance()->registerTracker(pWhiteTracker);

	TurnCounter* pTurnCount = new TurnCounter("TurnCounter", 20, Vector2D(220, 475), Vector2D(1, 1));
	GameObjectManager::getInstance()->addObject(pTurnCount);

	EndScreen* pEndScreen = new EndScreen("EndScreen");
	pEndScreen->setEnabled(false);
	GameObjectManager::getInstance()->addObject(pEndScreen);
}