#include "Title_Scene.h"
#include "Background.h"
#include "GUIButton.h"
#include "GameObjectManager.h"
#include "TextureManager.h"
#include "BubbleManager.h"
#include "EnumSceneTag.h"
#include "Sprite.h"
#include "AnimatedSprite.h"
#include "SceneSwitcher.h"
#include "AudioManager.h"
Title_Scene::Title_Scene()
    : AScene(SceneTag::TITLE_SCENE)
{
}

void Title_Scene::onLoadResources()
{
    // naming convention (TextureManager prefixes "Assets/")
    TextureManager::getInstance()->load("title_screen_pngs/Sprite_bg.png", "Title_Background");
    TextureManager::getInstance()->load("title_screen_pngs/mermaid.png", "Mermaid");
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
    AudioManager::getInstance()->load("sounds/Music/Music_Loop.wav", "Music");
}

void Title_Scene::onLoadObjects()
{
    Background* pBackground = new Background("Title_Background", "Title_Background", Vector2D(70.f, 1.17f));
    GameObjectManager::getInstance()->addObject((AGameObject*)pBackground);

    BubbleManager::initialize(50);

    Sprite* pObjectBG = new Sprite("Design_BG", "Design_BG", Vector2D(0, -330), Vector2D(2, 2), 0.0f, false);
    GameObjectManager::getInstance()->addObject((AGameObject*)pObjectBG);

    Sprite* pMermaid = new Sprite("Mermaid", "Mermaid",Vector2D(0, 320), Vector2D(0.7f), 0.0f, false);
    GameObjectManager::getInstance()->addObject((AGameObject*)pMermaid);

    Sprite* pLogo = new Sprite("Title_Logo", "Title_Logo",Vector2D(0, 150), Vector2D(0.3f), 0.0f, false);
    GameObjectManager::getInstance()->addObject((AGameObject*)pLogo);

    Sprite* pYellow = new Sprite("Yellow", "Yellow", Vector2D(-270, 300), Vector2D(0.25f), -45, false);
    GameObjectManager::getInstance()->addObject((AGameObject*)pYellow);

    Sprite* pFish = new Sprite("Blue", "Blue", Vector2D(-250, 150), Vector2D(0.25f), 25.0f, false);
    GameObjectManager::getInstance()->addObject((AGameObject*)pFish);

    Sprite* pPurple = new Sprite("Purple", "Purple", Vector2D(220, 460), Vector2D(0.25f), 45.0f, false);
    GameObjectManager::getInstance()->addObject((AGameObject*)pPurple);

    Sprite* pRed = new Sprite("Red", "Red", Vector2D(250, 150), Vector2D(0.25f), 0.0f, false);
    GameObjectManager::getInstance()->addObject((AGameObject*)pRed);

    Sprite* pWhite = new Sprite("White", "White", Vector2D(-220, 450), Vector2D(0.25f), 0.0f, false);
    GameObjectManager::getInstance()->addObject((AGameObject*)pWhite);

    Sprite* pGreen = new Sprite("Green", "Green", Vector2D(250, 270), Vector2D(0.25f), 0.0f, false);
    GameObjectManager::getInstance()->addObject((AGameObject*)pGreen);

    Sprite* pLevelSelect = new Sprite("Level_Select", "Button_Container", Vector2D(0, -290), Vector2D(2.5f), 0.0f, false);
    GameObjectManager::getInstance()->addObject((AGameObject*)pLevelSelect);


    GUIButton* pLevelOne = new GUIButton("Level_1_Button", "Level_Button");
    pLevelOne->setPos(Vector2D(-120, -220));
    pLevelOne->setScale(Vector2D(0.7f));
    GameObjectManager::getInstance()->addObject((AGameObject*)pLevelOne);
    SceneSwitcher* pLevelOneSwitcher = new SceneSwitcher(SceneTag::LEVEL_1_SCENE);
    pLevelOne->attachComponent(pLevelOneSwitcher);

    GUIButton* pLevelTwo = new GUIButton("Level_2_Button", "Level_Button");
    pLevelTwo->setPos(Vector2D(0, -220));
    pLevelTwo->setScale(Vector2D(0.7f));
    GameObjectManager::getInstance()->addObject((AGameObject*)pLevelTwo);
    SceneSwitcher* pLevelTwoSwitcher = new SceneSwitcher(SceneTag::LEVEL_2_SCENE);
    pLevelTwo->attachComponent(pLevelTwoSwitcher);

    GUIButton* pLevelThree = new GUIButton("Level_3_Button", "Level_Button");
    pLevelThree->setPos(Vector2D(120, -220));
    pLevelThree->setScale(Vector2D(0.7f));
    GameObjectManager::getInstance()->addObject((AGameObject*)pLevelThree);
    SceneSwitcher* pLevelThreeSwitcher = new SceneSwitcher(SceneTag::LEVEL_3_SCENE);
    pLevelThree->attachComponent(pLevelThreeSwitcher);
    
    if (!AudioManager::getInstance()->isPlaying("BGM"))
	    AudioManager::getInstance()->play(new AudioPlayer("Music", "BGM", AudioGroupTag::MUSIC, OnAudioFinished::LOOP));
}

void Title_Scene::onUnloadResources()
{
    TextureManager::getInstance()->unload("Title_Background");
    TextureManager::getInstance()->unload("Mermaid");
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
    AudioManager::getInstance()->unload("Music");
    // base implementation to remove all objects
    AScene::onUnloadObjects();
}
