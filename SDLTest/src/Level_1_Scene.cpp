#include "Level_1_Scene.h"

Level_1_Scene::Level_1_Scene():AScene(SceneTag::LEVEL_1_SCENE)
{
}

void Level_1_Scene::onLoadResources()
{
	TextureManager::getInstance()->load("title_screen_pngs/Sprite_bg.png", "Level_Background");
	TextureManager::getInstance()->load("title_screen_pngs/Sprite_Window.png", "Lower_UI_Container");
	TextureManager::getInstance()->load("Menu/Sprite_UI_setupicon.png", "Settings");


}

void Level_1_Scene::onLoadObjects()
{
	Background* pBackground = new Background("Level_Background", "Level_Background", Vector2D(125.f, 1.17f));
	GameObjectManager::getInstance()->addObject((AGameObject*)pBackground);

	Prop* pLowerUIContainer = new Prop("Lower_UI_Container", "Lower_UI_Container", Vector2D(-1000, 1000), Vector2D(20, 2.f), 0.0f, false);
	GameObjectManager::getInstance()->addObject((AGameObject*)pLowerUIContainer);

	UIButton* pSettings = new UIButton(
		"Settings",   // object name (can be unique)
		"Settings",          // texture key (MUST match TextureManager)
		Vector2D(1850, 1050),
		Vector2D(1.f, 1.f),
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
}

void Level_1_Scene::onUnloadObjects()
{
	AScene::onUnloadObjects();
}

void Level_1_Scene::update(float deltaTime)
{
	AScene::update(deltaTime);
}

void Level_1_Scene::render(SDL_Renderer* pRenderer)
{
	AScene::render(pRenderer);
}
