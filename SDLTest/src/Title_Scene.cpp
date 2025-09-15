#include "Title_Scene.h"
#include "Background.h"
#include "UIButton.h"
#include "GameObjectManager.h"
#include "TextureManager.h"
#include "EnumSceneTag.h"
#include "Prop.h"
#include "AnimatedSprite.h"

Title_Scene::Title_Scene()
    : AScene(SceneTag::TITLE_SCENE)
{
}

void Title_Scene::onLoadResources()
{
    // naming convention (TextureManager prefixes "Assets/")
    TextureManager::getInstance()->load("title_screen_pngs/Sprite_bg.png", "Title_Background");
    TextureManager::getInstance()->load("title_screen_pngs/Sprite_GemHunterMatch_Logo.png", "Title_Logo");
    TextureManager::getInstance()->load("title_screen_pngs/SP_Gem_Yellow.png", "Yellow");
    TextureManager::getInstance()->load("title_screen_pngs/SP_Gem_Blue.png", "Fish");
    TextureManager::getInstance()->load("title_screen_pngs/SP_Gem_Purple.png", "Purple");
    TextureManager::getInstance()->load("title_screen_pngs/SP_Gem_Red.png", "Red");
    TextureManager::getInstance()->load("title_screen_pngs/SP_Gem_White.png", "White");
    TextureManager::getInstance()->load("title_screen_pngs/SP_Gem_Green.png", "Green");
    TextureManager::getInstance()->load("title_screen_pngs/Sprite_Window.png", "Button_Container");
    TextureManager::getInstance()->load("title_screen_pngs/Sprite_LevelEntry_Bg.png", "Level_Button");
}

void Title_Scene::onLoadObjects()
{
    // create and register game objects for the title scene
    Background* pBackground = new Background("Title_Background", "Title_Background", Vector2D(125.f, 1.17f));
    Prop* pLogo = new Prop("Title_Logo", "Title_Logo", Vector2D(750.f, 400.f), Vector2D(0.5f, 0.35f), 0.0f, false);
    Prop* pYellow = new Prop("Yellow", "Yellow", Vector2D(650.f, 250.f), Vector2D(.35f, .35f), -45, false);
    Prop* pFish = new Prop("Fish", "Fish", Vector2D(450.f, 400.f), Vector2D(.35f, .35f), 25.0f, false);
    Prop* pPurple = new Prop("Purple", "Purple", Vector2D(1300.f, 300.f), Vector2D(.35f, .35f), 45.0f, false);
    Prop* pRed = new Prop("Red", "Red", Vector2D(350.f, 250.f), Vector2D(.35f, .35f), 0.0f, false);
    Prop* pWhite = new Prop("White", "White", Vector2D(500.f, 100.f), Vector2D(.35f, .35f), 0.0f, false);
    Prop* pGreen = new Prop("Green", "Green", Vector2D(1300.f, 500.f), Vector2D(.35f, .35f), 0.0f, false);
    Prop* pLevelSelect = new Prop("Level_Select", "Button_Container", Vector2D(350, 750.f), Vector2D(7, 3), 0.0f, false);

    // create a button that switches to the Lobby scene when clicked
    UIButton* pLevelOne = new UIButton(
        "Level_1_Button",   // object name (can be unique)
        "Level_Button",          // texture key (MUST match TextureManager)
        Vector2D(550.f, 825.f),
        Vector2D(2.f,2.f),
        0.0f, false,
        SceneTag::LOBBY_SCENE
    );
    UIButton* pLevelTwo = new UIButton(
        "Level_2_Button",   // object name (can be unique)
        "Level_Button",          // texture key (MUST match TextureManager)
        Vector2D(900.f, 825.f),
        Vector2D(2.f, 2.f),
        0.0f, false,
        SceneTag::LOBBY_SCENE
    );
    UIButton* pLevelThree = new UIButton(
        "Level_3_Button",   // object name (can be unique)
        "Level_Button",          // texture key (MUST match TextureManager)
        Vector2D(1250.f, 825.f),
        Vector2D(2.f, 2.f),
        0.0f, false,
        SceneTag::LOBBY_SCENE
    );

    GameObjectManager::getInstance()->addObject((AGameObject*)pBackground);
    GameObjectManager::getInstance()->addObject((AGameObject*)pLogo);
    GameObjectManager::getInstance()->addObject((AGameObject*)pYellow);
    GameObjectManager::getInstance()->addObject((AGameObject*)pFish);
    GameObjectManager::getInstance()->addObject((AGameObject*)pPurple);
    GameObjectManager::getInstance()->addObject((AGameObject*)pRed);
    GameObjectManager::getInstance()->addObject((AGameObject*)pWhite);
    GameObjectManager::getInstance()->addObject((AGameObject*)pGreen);
    GameObjectManager::getInstance()->addObject((AGameObject*)pLevelSelect);
    GameObjectManager::getInstance()->addObject((AGameObject*)pLevelOne);
    GameObjectManager::getInstance()->addObject((AGameObject*)pLevelTwo);
    GameObjectManager::getInstance()->addObject((AGameObject*)pLevelThree);

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
    TextureManager::getInstance()->unload("Button_Container");
    TextureManager::getInstance()->unload("Level_Button");
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
