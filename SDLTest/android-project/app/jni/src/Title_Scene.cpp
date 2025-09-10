#include "Title_Scene.h"
#include "Background.h"
#include "UIButton.h"
#include "GameObjectManager.h"
#include "TextureManager.h"
#include "EnumSceneTag.h"
#include "AnimatedSprite.h"

Title_Scene::Title_Scene()
    : AScene(SceneTag::TITLE_SCENE)
{
}

void Title_Scene::onLoadResources()
{
    // naming convention (TextureManager prefixes "Assets/")
    TextureManager::getInstance()->load("title_screen_pngs/title_screen_bg.png", "Title_Background");
    TextureManager::getInstance()->load("title_screen_pngs/title_button.png", "Start_Button");

    for (int i = 0; i < 6; i++)
    {
        std::string strPath = "animations/title_scene/player/Set" + std::to_string(i) + ".png";
        TextureManager::getInstance()->load(strPath, "Player");
    }

    for (int i = 0; i < 2; i++)
    {
        std::string strPath = "animations/title_scene/fairy/Set" + std::to_string(i) + ".png";
        TextureManager::getInstance()->load(strPath, "Fairy");
    }

    for (int i = 0; i < 7; i++)
    {
        std::string strPath = "animations/title_scene/librarian/Set" + std::to_string(i) + ".png";
        TextureManager::getInstance()->load(strPath, "Librarian");
    }

}

void Title_Scene::onLoadObjects()
{
    // create and register game objects for the title scene
    Background* pBackground = new Background("Title_Background", "Title_Background", 1,1);

    // create a button that switches to the Lobby scene when clicked
    UIButton* pStartButton = new UIButton(
        "Start_Button_Object",   // object name (can be unique)
        "Start_Button",          // texture key (MUST match TextureManager)
        250.f, 600.f,
        0.25f, 0.25f,
        0.0f, false,
        SceneTag::LOBBY_SCENE
    );

    AnimatedSprite* pPlayer = new AnimatedSprite("Player", "Player", -20.f, 100.f, 1.f, 1.f, 0.f, 0, 1, 4);
    AnimatedSprite* pFairy = new AnimatedSprite("Fairy", "Fairy", 50.f, 100.f, 1.f, 1.f, 0.f, 0, 1, 4);
    AnimatedSprite* pLibrarian = new AnimatedSprite("Librarian", "Librarian", 100.f, 300.f, 1.f, 1.f, 0.f, 0, 1, 4);

    GameObjectManager::getInstance()->addObject((AGameObject*)pBackground);
    GameObjectManager::getInstance()->addObject((AGameObject*)pStartButton);
    GameObjectManager::getInstance()->addObject((AGameObject*)pPlayer);
    GameObjectManager::getInstance()->addObject((AGameObject*)pLibrarian);
    GameObjectManager::getInstance()->addObject((AGameObject*)pFairy);
}

void Title_Scene::onUnloadResources()
{
    TextureManager::getInstance()->unload("Title_Background");
    TextureManager::getInstance()->unload("Start_Button");
    TextureManager::getInstance()->unload("Player");
    TextureManager::getInstance()->unload("Librarian");
    TextureManager::getInstance()->unload("Fairy");
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
