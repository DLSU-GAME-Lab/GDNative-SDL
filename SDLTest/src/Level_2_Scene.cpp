#include "Level_2_Scene.h"
#include "Prop.h"
#include "GemManager.h"

Level_2_Scene::Level_2_Scene():AScene(SceneTag::LEVEL_2_SCENE)
{
}

void Level_2_Scene::onLoadResources()
{
	TextureManager::getInstance()->load("title_screen_pngs/Sprite_bg.png", "Level_Background");
	TextureManager::getInstance()->load("title_screen_pngs/Sprite_Window.png", "UI_Container");
	TextureManager::getInstance()->load("Menu/Sprite_UI_setupicon.png", "Settings");
	TextureManager::getInstance()->load("title_screen_pngs/Background_Objects.png", "Design_BG");
	TextureManager::getInstance()->load("levels/Level_2.png", "Level_Container");
	TextureManager::getInstance()->load("Menu/Sprite_UI_TopBar_BG_2.png", "Top_UI_Container");
	TextureManager::getInstance()->load("Menu/Sprite_UI_TopBar_BG_round.png", "Level_Container_Extra");
	GemManager::loadResources();
}

void Level_2_Scene::onLoadObjects()
{
	Background* pBackground = new Background("Level_Background", "Level_Background", Vector2D(125.f, 1.17f));
	GameObjectManager::getInstance()->addObject((AGameObject*)pBackground);

	Prop* pObjectBG = new Prop("Design_BG", "Design_BG", Vector2D(0, -330), Vector2D(2, 2), 0.0f, false);
	GameObjectManager::getInstance()->addObject((AGameObject*)pObjectBG);

	Prop* pLevelContainer = new Prop("Level_Container", "Level_Container", Vector2D(0, -20), Vector2D(2, 2), 0.0f, false);
	GameObjectManager::getInstance()->addObject((AGameObject*)pLevelContainer);

	Prop* pLowerUIContainer = new Prop("Lower_UI_Container", "UI_Container", Vector2D(0, -540), Vector2D(20, 2.f), 0.0f, false);
	GameObjectManager::getInstance()->addObject((AGameObject*)pLowerUIContainer);

	Prop* pUpperUIContainer = new Prop("Upper_UI_Container", "Top_UI_Container", Vector2D(0, 520), Vector2D(500, 1.f), 0.0f, false);
	GameObjectManager::getInstance()->addObject((AGameObject*)pUpperUIContainer);

	Prop* pUIContainerExtra = new Prop("Level_Container_Extra", "Level_Container_Extra", Vector2D(0, 547), Vector2D(1, 1), 0.0f, false);
	GameObjectManager::getInstance()->addObject((AGameObject*)pUIContainerExtra);

	UIButton* pSettings = new UIButton(
		"Settings",   // object name (can be unique)
		"Settings",          // texture key (MUST match TextureManager)
		Vector2D(910, -490),
		Vector2D(.75f, .75f),
		0.0f, false,
		SceneTag::TITLE_SCENE
	);
	GameObjectManager::getInstance()->addObject((AGameObject*)pSettings);

	GemManager::initialize(10, 8, 60.0f, Vector2D(21.0f, -54.0f));

	std::vector<Uint64> r0Cols = { 4, 5 };
	std::vector<Uint64> r0r6Cols = { 0, 1, 8, 9 };
	std::vector<Uint64> r1r5Cols = { 0, 9 };
	std::vector<Uint64> r7Cols = { 0, 1, 2, 7, 8, 9 };

	GemManager::getInstance()->setBlockedCells(0, r0Cols, true);
	GemManager::getInstance()->setBlockedCells(0, r0r6Cols, true);
	GemManager::getInstance()->setBlockedCells(1, r1r5Cols, true);
	GemManager::getInstance()->setBlockedCells(5, r1r5Cols, true);
	GemManager::getInstance()->setBlockedCells(6, r0r6Cols, true);
	GemManager::getInstance()->setBlockedCells(7, r7Cols, true);

	GemManager::getInstance()->spawnGems(0.2f);
}

void Level_2_Scene::onUnloadResources()
{
	TextureManager::getInstance()->unload("Level_Background");
	TextureManager::getInstance()->unload("UI_Container");
	TextureManager::getInstance()->unload("Settings");
	TextureManager::getInstance()->unload("Design_BG");
	TextureManager::getInstance()->unload("Level_Container");
	TextureManager::getInstance()->unload("Top_UI_Container");
	TextureManager::getInstance()->unload("Level_Container_Extra");
	GemManager::unloadResources();
}

void Level_2_Scene::onUnloadObjects()
{
	AScene::onUnloadObjects();
}
