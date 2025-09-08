#include "Title_Scene.h"
#include "Background.h"
#include "UIButton.h"
#include "GameObjectManager.h"
#include "TextureManager.h"
#include "EnumSceneTag.h"

Title_Scene::Title_Scene()
    : AScene(SceneTag::TITLE_SCENE)
{
}

void Title_Scene::onLoadResources()
{
    // naming convention (TextureManager prefixes "Assets/")
    TextureManager::getInstance()->load("title_screen_pngs/title_screen_bg.png", "Title_Background");
    TextureManager::getInstance()->load("title_screen_pngs/title_button.png", "Start_Button");
}

void Title_Scene::onLoadObjects()
{
    // create and register game objects for the title scene
    Background* pBackground = new Background("Title_Background", "Title_Background");

    // create a button that switches to the Lobby scene when clicked
    UIButton* pStartButton = new UIButton("StartButton", 800, 500, 300, 150, false, SceneTag::LOBBY_SCENE);

    GameObjectManager::getInstance()->addObject((AGameObject*)pBackground);
    GameObjectManager::getInstance()->addObject((AGameObject*)pStartButton);
}

void Title_Scene::onUnloadResources()
{
    TextureManager::getInstance()->unload("Title_Background");
    TextureManager::getInstance()->unload("Start_Button");
}

void Title_Scene::onUnloadObjects()
{
    // base implementation to remove all objects
    AScene::onUnloadObjects();
}

void Title_Scene::update(float deltaTime)
{
    // defer to base (which updates GameObjectManager)
    AScene::update(deltaTime);
}

void Title_Scene::render(SDL_Renderer* pRenderer)
{
    // base render will call SpriteRendererSystem::draw()
    AScene::render(pRenderer);
}
