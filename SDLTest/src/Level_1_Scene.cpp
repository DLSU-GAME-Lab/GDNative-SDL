#include "Level_1_Scene.h"
#include "GemManager.h"

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
	TextureManager::getInstance()->load("title_screen_pngs/Background_Objects.png", "Design_BG");
	TextureManager::getInstance()->load("levels/Level_1.png", "Level_Container");
	GemManager::loadResources();
}

void Level_1_Scene::onLoadObjects()
{
	Background* pBackground = new Background("Level_Background", "Level_Background", Vector2D(125.f, 1.17f));
	GameObjectManager::getInstance()->addObject((AGameObject*)pBackground);

	Prop* pObjectBG = new Prop("Design_BG", "Design_BG", Vector2D(0, -330), Vector2D(2, 2), 0.0f, false);
	GameObjectManager::getInstance()->addObject((AGameObject*)pObjectBG);

	Prop* pLevelContainer = new Prop("Level_Container", "Level_Container", Vector2D(0, -20), Vector2D(2, 2), 0.0f, false);
	GameObjectManager::getInstance()->addObject((AGameObject*)pLevelContainer);

	Prop* pLowerUIContainer = new Prop("Lower_UI_Container", "UI_Container", Vector2D(0, -540), Vector2D(20, 2.f), 0.0f, false);
	GameObjectManager::getInstance()->addObject((AGameObject*)pLowerUIContainer);

	Prop* pUpperUIContainer = new Prop("Upper_UI_Container", "Top_UI_Container", Vector2D(0, 520), Vector2D(500, 1), 0.0f, false);
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

	GemManager::initialize(9, 10, 60.0f, Vector2D(30.0f, -54.0f));

	std::vector<Uint8> r0Cols = { 0, 8 };
	std::vector<Uint8> r9Cols = { 3, 4, 5 };

	GemManager::getInstance()->setBlockedCells(0, r0Cols, true);
	GemManager::getInstance()->setBlockedCells(9, r9Cols, true);

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
	GemManager::unloadResources();
}

void Level_1_Scene::onUnloadObjects()
{
	AScene::onUnloadObjects();
}
