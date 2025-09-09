#include "Title_Scene.h"
#include "Background.h"
#include "UIButton.h"
#include "GameObjectManager.h"
#include "TextureManager.h"
#include "EnumSceneTag.h"
#include "Prop.h"
Title_Scene::Title_Scene()
    : AScene(SceneTag::TITLE_SCENE)
{
}

void Title_Scene::onLoadResources()
{
    // naming convention (TextureManager prefixes "Assets/")
    TextureManager::getInstance()->load("title_screen_pngs/Sprite_bg.png", "Title_Background");
    TextureManager::getInstance()->load("title_screen_pngs/Sprite_GemHunterMatch_Logo.png", "Title_Logo");
    TextureManager::getInstance()->load("title_screen_pngs/Sprite_YellowItem.png", "Yellow");
    TextureManager::getInstance()->load("title_screen_pngs/Sprite_BlueItem.png", "Fish");
    TextureManager::getInstance()->load("title_screen_pngs/Sprite_PurpleItem.png", "Purple");
    TextureManager::getInstance()->load("title_screen_pngs/Sprite_RedItem.png", "Red");
    TextureManager::getInstance()->load("title_screen_pngs/Sprite_WhiteItem.png", "White");
    TextureManager::getInstance()->load("title_screen_pngs/Sprite_GreenItem.png", "Green");
}

void Title_Scene::onLoadObjects()
{
    // create and register game objects for the title scene
    Background* pBackground = new Background("Title_Background", "Title_Background", 125.f,1.17f);
    Prop* pLogo = new Prop("Title_Logo", "Title_Logo", 750.f, 400.f, 0.5f, 0.35f, 0.0f, false);
    Prop* pYellow = new Prop("Yellow", "Yellow", 650.f, 250.f, .25f, .25f, -45, false);
    Prop* pFish = new Prop("Fish", "Fish", 450.f, 400.f, .25f, .25f, 25.0f, false);
    Prop* pPurple = new Prop("Purple", "Purple", 1300.f, 300.f, .25f, .25f, 45.0f, false);
    Prop* pRed = new Prop("Red", "Red", 350.f, 250.f, .25f, .25f, 0.0f, false);
    Prop* pWhite = new Prop("White", "White", 500.f, 100.f, .25f, .25f, 0.0f, false);
    Prop* pGreen = new Prop("Green", "Green", 1300.f, 500.f, .25f, .25f, 0.0f, false);

    // create a button that switches to the Lobby scene when clicked
    //UIButton* pStartButton = new UIButton(
    //    "Start_Button_Object",   // object name (can be unique)
    //    "Start_Button",          // texture key (MUST match TextureManager)
    //    250.f, 600.f,
    //    0.25f, 0.25f,
    //    0.0f, false,
    //    SceneTag::LOBBY_SCENE
    //);

    //GameObjectManager::getInstance()->addObject((AGameObject*)pStartButton);
    GameObjectManager::getInstance()->addObject((AGameObject*)pBackground);
    GameObjectManager::getInstance()->addObject((AGameObject*)pLogo);
    GameObjectManager::getInstance()->addObject((AGameObject*)pYellow);
    GameObjectManager::getInstance()->addObject((AGameObject*)pFish);
    GameObjectManager::getInstance()->addObject((AGameObject*)pPurple);
    GameObjectManager::getInstance()->addObject((AGameObject*)pRed);
    GameObjectManager::getInstance()->addObject((AGameObject*)pWhite);
    GameObjectManager::getInstance()->addObject((AGameObject*)pGreen);
}

void Title_Scene::onUnloadResources()
{
    TextureManager::getInstance()->unload("Title_Background");
    TextureManager::getInstance()->unload("Title_Logo");
    TextureManager::getInstance()->unload("Yellow");
    TextureManager::getInstance()->unload("Fish");
    TextureManager::getInstance()->unload("Green");
    TextureManager::getInstance()->unload("Purple");
    TextureManager::getInstance()->unload("Red");
    TextureManager::getInstance()->unload("White");
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
