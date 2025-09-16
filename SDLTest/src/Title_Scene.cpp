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
    Background* pBackground = new Background("Title_Background", "Title_Background", Vector2D(1.f,1.f));
    GameObjectManager::getInstance()->addObject((AGameObject*)pBackground);

    AnimatedSprite* pLibrarian = new AnimatedSprite("Librarian", "Librarian", Vector2D(0.f, 300.f), Vector2D(1.f, 1.f), 0.f, 8);
    GameObjectManager::getInstance()->addObject((AGameObject*)pLibrarian);

    AnimatedSprite* pFairy = new AnimatedSprite("Fairy", "Fairy", Vector2D(-50.f, 150.f), Vector2D(1.f, 1.f), 0.f, 8);
    GameObjectManager::getInstance()->addObject((AGameObject*)pFairy);

    AnimatedSprite* pPlayer = new AnimatedSprite("Player", "Player", Vector2D(-300.f, 0.f), Vector2D(1.f, 1.f), 0.f, 8);
    GameObjectManager::getInstance()->addObject((AGameObject*)pPlayer);

    // create a button that switches to the Lobby scene when clicked
    UIButton* pStartButton = new UIButton(
        "Start_Button_Object",   // object name (can be unique)
        "Start_Button",          // texture key (MUST match TextureManager)
        Vector2D(60.f, 800.f),
        Vector2D(0.25f, 0.25f),
        0.0f, false,
        SceneTag::LOBBY_SCENE
    );
    GameObjectManager::getInstance()->addObject((AGameObject*)pStartButton);

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
