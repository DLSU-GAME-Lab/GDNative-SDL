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
    TextureManager::getInstance()->load("gems/SP_Gem_Blue.png", "Blue");
    TextureManager::getInstance()->load("gems/SP_Gem_Purple.png", "Purple");
    TextureManager::getInstance()->load("gems/SP_Gem_Red.png", "Red");
    TextureManager::getInstance()->load("gems/SP_Gem_White.png", "White");
    TextureManager::getInstance()->load("gems/SP_Gem_Green.png", "Green");
    TextureManager::getInstance()->load("title_screen_pngs/Sprite_Window.png", "Button_Container");
    TextureManager::getInstance()->load("title_screen_pngs/Sprite_LevelEntry_Bg.png", "Level_Button");
    TextureManager::getInstance()->load("title_screen_pngs/Background_Objects.png", "Design_BG");
}

void Title_Scene::onLoadObjects()
{
    Background* pBackground = new Background("Title_Background", "Title_Background", Vector2D(70.f, 1.17f));
    GameObjectManager::getInstance()->addObject((AGameObject*)pBackground);

    Prop* pObjectBG = new Prop("Design_BG", "Design_BG", Vector2D(0, -330), Vector2D(2, 2), 0.0f, false);
    GameObjectManager::getInstance()->addObject((AGameObject*)pObjectBG);

    Prop* pLogo = new Prop("Title_Logo", "Title_Logo",Vector2D(0, 200), Vector2D(0.5f, 0.35f), 0.0f, false);
    GameObjectManager::getInstance()->addObject((AGameObject*)pLogo);

    Prop* pYellow = new Prop("Yellow", "Yellow", Vector2D(-320, 300), Vector2D(.35f, .35f), -45, false);
    GameObjectManager::getInstance()->addObject((AGameObject*)pYellow);

    Prop* pFish = new Prop("Blue", "Blue", Vector2D(-450, 150), Vector2D(.35f, .35f), 25.0f, false);
    GameObjectManager::getInstance()->addObject((AGameObject*)pFish);

    Prop* pPurple = new Prop("Purple", "Purple", Vector2D(470, 360), Vector2D(.35f, .35f), 45.0f, false);
    GameObjectManager::getInstance()->addObject((AGameObject*)pPurple);

    Prop* pRed = new Prop("Red", "Red", Vector2D(450, 150), Vector2D(.35f, .35f), 0.0f, false);
    GameObjectManager::getInstance()->addObject((AGameObject*)pRed);

    Prop* pWhite = new Prop("White", "White", Vector2D(-420, 450), Vector2D(.35f, .35f), 0.0f, false);
    GameObjectManager::getInstance()->addObject((AGameObject*)pWhite);

    Prop* pGreen = new Prop("Green", "Green", Vector2D(350, 200), Vector2D(.35f, .35f), 0.0f, false);
    GameObjectManager::getInstance()->addObject((AGameObject*)pGreen);

    Prop* pLevelSelect = new Prop("Level_Select", "Button_Container", Vector2D(0, -240), Vector2D(7, 3), 0.0f, false);
    GameObjectManager::getInstance()->addObject((AGameObject*)pLevelSelect);


    UIButton* pLevelOne = new UIButton(
        "Level_1_Button",   // object name (can be unique)
        "Level_Button",          // texture key (MUST match TextureManager)
        Vector2D(-360, -230),
        Vector2D(2.f, 2.f),
        0.0f, false,
        SceneTag::LEVEL_1_SCENE
    );
    GameObjectManager::getInstance()->addObject((AGameObject*)pLevelOne);

    UIButton* pLevelTwo = new UIButton(
        "Level_2_Button",   // object name (can be unique)
        "Level_Button",          // texture key (MUST match TextureManager)
        Vector2D(0, -230),
        Vector2D(2.f, 2.f),
        0.0f, false,
        SceneTag::LEVEL_2_SCENE
    );
    GameObjectManager::getInstance()->addObject((AGameObject*)pLevelTwo);

    UIButton* pLevelThree = new UIButton(
        "Level_3_Button",   // object name (can be unique)
        "Level_Button",          // texture key (MUST match TextureManager)
        Vector2D(360, -230),
        Vector2D(2.f, 2.f),
        0.0f, false,
        SceneTag::LEVEL_3_SCENE
    );
    GameObjectManager::getInstance()->addObject((AGameObject*)pLevelThree);

}

void Title_Scene::onUnloadResources()
{
    TextureManager::getInstance()->unload("Title_Background");
    TextureManager::getInstance()->unload("Title_Logo");
    TextureManager::getInstance()->unload("Yellow");
    TextureManager::getInstance()->unload("Blue");
    TextureManager::getInstance()->unload("Green");
    TextureManager::getInstance()->unload("Purple");
    TextureManager::getInstance()->unload("Red");
    TextureManager::getInstance()->unload("White");
    TextureManager::getInstance()->unload("Button_Container");
    TextureManager::getInstance()->unload("Level_Button");
    TextureManager::getInstance()->unload("Design_BG");
}

void Title_Scene::onUnloadObjects()
{
    // base implementation to remove all objects
    AScene::onUnloadObjects();
}
