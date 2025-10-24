#include "Title_Scene.h"
#include "Background.h"
#include "GUIButton.h"
#include "GameObjectManager.h"
#include "TextureManager.h"
#include "BubbleManager.h"
#include "EnumSceneTag.h"
#include "Prop.h"
#include "AnimatedSprite.h"
#include "SceneSwitcher.h"
Title_Scene::Title_Scene()
    : AScene(SceneTag::TITLE_SCENE)
{
}

void Title_Scene::onLoadResources()
{
    // naming convention (TextureManager prefixes "Assets/")
    TextureManager::getInstance()->load("title_screen_pngs/Sprite_bg.png", "Title_Background");
    TextureManager::getInstance()->load("title_screen_pngs/Sprite_GemHunterMatch_Logo.png", "Title_Logo");
    TextureManager::getInstance()->load("gems/SP_Gem_Yellow.png", "Yellow");
    TextureManager::getInstance()->load("gems/SP_Gem_Blue.png", "Blue");
    TextureManager::getInstance()->load("gems/SP_Gem_Purple.png", "Purple");
    TextureManager::getInstance()->load("gems/SP_Gem_Red.png", "Red");
    TextureManager::getInstance()->load("gems/SP_Gem_White.png", "White");
    TextureManager::getInstance()->load("gems/SP_Gem_Green.png", "Green");
    TextureManager::getInstance()->load("title_screen_pngs/Sprite_Window.png", "Button_Container");
    TextureManager::getInstance()->load("title_screen_pngs/Sprite_LevelEntry_Bg.png", "Level_Button");
    TextureManager::getInstance()->load("title_screen_pngs/Background_Objects.png", "Design_BG");
    TextureManager::getInstance()->load("Sprite_Bubble_Small.png", "Bubble");
}

void Title_Scene::onLoadObjects()
{
    Background* pBackground = new Background("Title_Background", "Title_Background", Vector2D(70.f, 1.17f));
    GameObjectManager::getInstance()->addObject((AGameObject*)pBackground);

    BubbleManager::initialize(50);

    Prop* pObjectBG = new Prop("Design_BG", "Design_BG", Vector2D(0, -330), Vector2D(2, 2), 0.0f, false);
    GameObjectManager::getInstance()->addObject((AGameObject*)pObjectBG);

    Prop* pLogo = new Prop("Title_Logo", "Title_Logo",Vector2D(0, 250), Vector2D(0.5f, 0.5f), 0.0f, false);
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

    Prop* pLevelSelect = new Prop("Level_Select", "Button_Container", Vector2D(0, -210), Vector2D(3.5f, 2.0f), 0.0f, false);
    GameObjectManager::getInstance()->addObject((AGameObject*)pLevelSelect);


    GUIButton* pLevelOne = new GUIButton("Level_1_Button", "Level_Button");
    pLevelOne->setPos(Vector2D(-150, -200));
    GameObjectManager::getInstance()->addObject((AGameObject*)pLevelOne);
    SceneSwitcher* pLevelOneSwitcher = new SceneSwitcher(SceneTag::LEVEL_1_SCENE);
    pLevelOne->attachComponent(pLevelOneSwitcher);

    GUIButton* pLevelTwo = new GUIButton("Level_2_Button", "Level_Button");
    pLevelTwo->setPos(Vector2D(0, -200));
    GameObjectManager::getInstance()->addObject((AGameObject*)pLevelTwo);
    SceneSwitcher* pLevelTwoSwitcher = new SceneSwitcher(SceneTag::LEVEL_2_SCENE);
    pLevelTwo->attachComponent(pLevelTwoSwitcher);

    GUIButton* pLevelThree = new GUIButton("Level_3_Button", "Level_Button");
    pLevelThree->setPos(Vector2D(150, -200));
    GameObjectManager::getInstance()->addObject((AGameObject*)pLevelThree);
    SceneSwitcher* pLevelThreeSwitcher = new SceneSwitcher(SceneTag::LEVEL_3_SCENE);
    pLevelThree->attachComponent(pLevelThreeSwitcher);
  
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
    TextureManager::getInstance()->unload("Bubble");
}

void Title_Scene::onUnloadObjects()
{
    // base implementation to remove all objects
    AScene::onUnloadObjects();
}
