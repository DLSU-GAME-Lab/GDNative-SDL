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
#include "Sprite.h"
#include "AudioManager.h"
#include "TweenAnimator.h"
#include "Lightmap.h"
#include "RadialLight.h"

Title_Scene::Title_Scene()
    : AScene(SceneTag::TITLE_SCENE)
{
}

void Title_Scene::onLoadResources()
{
    this->loadText();
    this->loadAnimatedTextures();
    this->loadSceneTextures();
	AudioManager::getInstance()->load("sounds/Music/TitleScreen_Song.wav", "TitleScreen_Music");
}

void Title_Scene::onLoadObjects()
{
    CameraManager::getInstance()->getCurrentCamera()->setPos(Vector2D(0));
    // create and register game objects for the title scene
    this->createScene();
    this->createButtons();

	AudioPlayer* pBGM = new AudioPlayer("TitleScreen_Music", "Title_BGM", AudioGroupTag::MUSIC, OnAudioFinished::LOOP);
	AudioManager::getInstance()->play(pBGM);

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
    FontManager::getInstance()->unloadAllFonts();
}

void Title_Scene::onUnloadObjects()
{
    // base implementation to remove all objects
	AudioManager::getInstance()->stopAll();
    AScene::onUnloadObjects();
}

void Title_Scene::loadText()
{
    //FontManager::getInstance()->loadFont("lazy.ttf", "LazyFont90", 90);
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
    TextureManager::getInstance()->load("GUI/title_button.png", "Start_Button");
    TextureManager::getInstance()->load("title_screen_pngs/game_logo.png", "Title_Banner");
    TextureManager::getInstance()->load("title_screen_pngs/DLSU_logos.png", "DLSU_Logos");
}

void Title_Scene::createButtons()
{
    GUIButton* pStartButton = new GUIButton("Start_Button", "Start_Button");
    pStartButton->setPos(Vector2D(0, 0));
    pStartButton->setScale(Vector2D(0.2f, 0.2f));

    GUIButton* pResetButton = new GUIButton("Reset_Button", "Start_Button");
    pResetButton->setPos(Vector2D(0, 0));
    pResetButton->setScale(Vector2D(0.2f, 0.2f));

    SceneSwitcher* pSceneSwitcher = new SceneSwitcher(SceneTag::LOBBY_SCENE);
    pStartButton->attachComponent(pSceneSwitcher);
    GameObjectManager::getInstance()->addObject(pStartButton);

    GameObjectManager::getInstance()->addObject(pResetButton);

    SDL_Color color = { 127, 31, 31, 255 };

    Text* pStartText = new Text("Start_Text", "JainiPurva-Regular.ttf", 90, 0.f, false);
    pStartButton->attachChild(pStartText);
    pStartText->setMessage("START");
    pStartText->setColor(color);
    pStartText->setScale(Vector2D(1,1));
    pStartButton->setPos(Vector2D(-580, -100));

    Text* pResetText = new Text("Reset_Text", "JainiPurva-Regular.ttf", 90, 0.f, false);
    pResetButton->attachChild(pResetText);
    pResetText->setMessage("RESET");
    pResetText->setColor(color);
    pResetText->setScale(Vector2D(1,1));
    pResetButton->setPos(Vector2D(-580, -300));

}

void Title_Scene::createScene()
{ 
    Background* pBackground = new Background("Title_Background", "Title_Background", Vector2D(1.f, 1.f));
    pBackground->setPos(Vector2D(-120.0f, 0.0f));
    GameObjectManager::getInstance()->addObject(pBackground);

    AnimatedSprite* pLibrarian = new AnimatedSprite("Librarian", "Librarian", Vector2D(-50.f, -90.f), Vector2D(1.f, 1.f), 0.f, 8);
    GameObjectManager::getInstance()->addObject(pLibrarian);

    AnimatedSprite* pFairy = new AnimatedSprite("Fairy", "Fairy", Vector2D(-100.f, 50.f), Vector2D(1.f, 1.f), 0.f, 8);
    GameObjectManager::getInstance()->addObject(pFairy);

    AnimatedSprite* pPlayer = new AnimatedSprite("Player", "Player", Vector2D(-100.f, 0.f), Vector2D(.9f, .9f), 0.f, 8);
    GameObjectManager::getInstance()->addObject(pPlayer);

    Lightmap::initialize();
    Lightmap::getInstance()->setAmbientColor({51, 46, 41, 255});
    RadialLight* pLight = new RadialLight("Light", 1000);
    pLight->setPos(Vector2D(540, 0));
    GameObjectManager::getInstance()->addObject(pLight);

    Sprite* pLogo = new Sprite("Game_Logo", "Title_Banner", Vector2D(-550, 250), Vector2D(1.f, 1.f), 0.f, false);
    GameObjectManager::getInstance()->addObject(pLogo);

    Vector2D start = Vector2D(-550, 250);
    Vector2D end = Vector2D(-550, 300);
    TweenAnimator* pTween = new TweenAnimator();
    pTween->setAnimationType(AnimationType::YOYO);
    pTween->setTweenPos(Tween2D::from(start.x, start.y).to(end.x, end.y).during(3000).via(tweeny::easing::quadraticInOut));
    pTween->play();
    pLogo->attachComponent(pTween);

    Sprite* pDLSULogo = new Sprite("DLSU_Logos", "DLSU_Logos", Vector2D(580, -420), Vector2D(.75f, .75f), 0.f, false);
    GameObjectManager::getInstance()->addObject(pDLSULogo);

}
