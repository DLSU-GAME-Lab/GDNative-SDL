#include "Title_Scene.h"
#include "Background.h"
#include "GameObjectManager.h"
#include "TextureManager.h"
#include "EnumSceneTag.h"
#include "AnimatedSprite.h"
#include "FontManager.h"
#include "Settings.h"
#include "Text.h"
#include "GUIButton.h"
#include "SceneSwitcher.h"
#include "Prop.h"

Title_Scene::Title_Scene()
    : AScene(SceneTag::TITLE_SCENE)
{
}

void Title_Scene::onLoadResources()
{
    this->loadText();
    this->loadAnimatedTextures();
    this->loadSceneTextures();
}

void Title_Scene::onLoadObjects()
{
    // create and register game objects for the title scene
    this->createScene();
    this->createButtons();

    //// create a button that switches to the Lobby scene when clicked
    //UIButton* pStartButton = new UIButton(
    //    "Start_Button_Object",   // object name (can be unique)
    //    "Start_Button",          // texture key (MUST match TextureManager)
    //    Vector2D(-580.f, 15.f),
    //    Vector2D(0.25f, 0.25f),
    //    0.0f, false,
    //    SceneTag::LOBBY_SCENE
    //);
    //GameObjectManager::getInstance()->addObject((AGameObject*)pStartButton);




}

void Title_Scene::onUnloadResources()
{
    TextureManager::getInstance()->unload("Title_Background");
    TextureManager::getInstance()->unload("Start_Button");
    TextureManager::getInstance()->unload("Player");
    TextureManager::getInstance()->unload("Librarian");
    TextureManager::getInstance()->unload("Fairy");
    TextureManager::getInstance()->unload("Title_Banner");
    TextureManager::getInstance()->unload("DLSU_Logos");
    FontManager::getInstance()->unloadFont("LazyFont");
}

void Title_Scene::onUnloadObjects()
{
    // base implementation to remove all objects
    AScene::onUnloadObjects();
}

void Title_Scene::loadText()
{
    FontManager::getInstance()->loadFont("lazy.ttf", "LazyFont90", 90);
}
void Title_Scene::loadAnimatedTextures()
{
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

void Title_Scene::loadSceneTextures()
{
    TextureManager::getInstance()->load("title_screen_pngs/title_screen_bg.png", "Title_Background");
    TextureManager::getInstance()->load("title_screen_pngs/title_button.png", "Start_Button");
    TextureManager::getInstance()->load("title_screen_pngs/game_logo.png", "Title_Banner");
    TextureManager::getInstance()->load("title_screen_pngs/DLSU_logos.png", "DLSU_Logos");
}

void Title_Scene::createButtons()
{
    GUIButton* pStartButton = new GUIButton("Start_Button", "Start_Button");
    //GUIUtils::setGUIMidLeft(pStartButton, Vector2D(20.0f, 0.0f));
    pStartButton->setPos(Vector2D(0, 0));
    pStartButton->setScale(Vector2D(0.25f, 0.25f));

    SceneSwitcher* pSceneSwitcher = new SceneSwitcher(SceneTag::LOBBY_SCENE);
    pStartButton->attachComponent(pSceneSwitcher);
    GameObjectManager::getInstance()->addObject(pStartButton);
    Text* pStartText = new Text("Start_Text", "Start Game", Vector2D(0, 0), Vector2D(1, 1), 0.f, false);
    pStartText->setFont("LazyFont90");
    pStartButton->attachChild(pStartText);
    GameObjectManager::getInstance()->addObject(pStartText);
    pStartButton->setPos(Vector2D(-580, -150));

}

void Title_Scene::createScene()
{ 
    Background* pBackground = new Background("Title_Background", "Title_Background", Vector2D(1.f, 1.f));
    GameObjectManager::getInstance()->addObject(pBackground);

    AnimatedSprite* pLibrarian = new AnimatedSprite("Librarian", "Librarian", Vector2D(-50.f, -240), Vector2D(1.f, 1.f), 0.f, 8);
    GameObjectManager::getInstance()->addObject(pLibrarian);

    AnimatedSprite* pFairy = new AnimatedSprite("Fairy", "Fairy", Vector2D(-100.f, -150.f), Vector2D(1.f, 1.f), 0.f, 8);
    GameObjectManager::getInstance()->addObject(pFairy);

    AnimatedSprite* pPlayer = new AnimatedSprite("Player", "Player", Vector2D(-300, 60.f), Vector2D(1.f, 1.f), 0.f, 8);
    GameObjectManager::getInstance()->addObject(pPlayer);

    Prop* pLogo = new Prop("Game_Logo", "Title_Banner", Vector2D(-550, 200), Vector2D(1.f, 1.f), 0.f, false);
    GameObjectManager::getInstance()->addObject(pLogo);

    Prop* pDLSULogo = new Prop("DLSU_Logos", "DLSU_Logos", Vector2D(580, -420), Vector2D(.75f, .75f), 0.f, false);
    GameObjectManager::getInstance()->addObject(pDLSULogo);

}
