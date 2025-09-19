#include "Level_1_Scene.h"

Level_1_Scene::Level_1_Scene():AScene(SceneTag::LEVEL_1_SCENE)
{
}

void Level_1_Scene::onLoadResources()
{
	TextureManager::getInstance()->load("title_screen_pngs/Sprite_bg.png", "Level_Background");
	TextureManager::getInstance()->load("title_screen_pngs/Sprite_Window.png", "Lower_UI_Container");
	TextureManager::getInstance()->load("Menu/Sprite_UI_setupicon.png", "Settings");
	TextureManager::getInstance()->load("title_screen_pngs/Background_Objects.png", "Design_BG");
	TextureManager::getInstance()->load("levels/Level_1.png", "Level_Container");




}

void Level_1_Scene::onLoadObjects()
{
	Background* pBackground = new Background("Level_Background", "Level_Background", Vector2D(125.f, 1.17f));
	GameObjectManager::getInstance()->addObject((AGameObject*)pBackground);

	Prop* pObjectBG = new Prop("Design_BG", "Design_BG", Vector2D(0, -330), Vector2D(2, 2), 0.0f, false);
	GameObjectManager::getInstance()->addObject((AGameObject*)pObjectBG);

	Prop* pLevelContainer = new Prop("Level_Container", "Level_Container", Vector2D(0, -20), Vector2D(2, 2), 0.0f, false);
	GameObjectManager::getInstance()->addObject((AGameObject*)pLevelContainer);

	Prop* pLowerUIContainer = new Prop("Lower_UI_Container", "Lower_UI_Container", Vector2D(0, -540), Vector2D(20, 2.f), 0.0f, false);
	GameObjectManager::getInstance()->addObject((AGameObject*)pLowerUIContainer);

	UIButton* pSettings = new UIButton(
		"Settings",   // object name (can be unique)
		"Settings",          // texture key (MUST match TextureManager)
		Vector2D(910, -490),
		Vector2D(.75f, .75f),
		0.0f, false,
		SceneTag::TITLE_SCENE
	);
	GameObjectManager::getInstance()->addObject((AGameObject*)pSettings);

}

void Level_1_Scene::onUnloadResources()
{
	TextureManager::getInstance()->unload("Level_Background");
	TextureManager::getInstance()->unload("Lower_UI_Container");
	TextureManager::getInstance()->unload("Settings");
	TextureManager::getInstance()->unload("Design_BG");
	TextureManager::getInstance()->unload("Level_Container");

}

void Level_1_Scene::onUnloadObjects()
{
	AScene::onUnloadObjects();
}
