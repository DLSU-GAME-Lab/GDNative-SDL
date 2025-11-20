#include "RightRoomScene.h"
#include "Background.h"
#include "Player.h"
#include "Sprite.h"
#include "GUIButton.h"
#include "SceneSwitcher.h"
#include "FontManager.h"
#include "Text.h"
#include "GUIToggle.h"
#include "Settings.h"
#include "EmptyObject.h"
#include "SpriteAnimator.h"
#include "Animation.h"
#include "AnimatedSprite.h"
#include "AudioManager.h"
RightRoomScene::RightRoomScene():AScene(SceneTag::RIGHT_ROOM_SCENE)
{
}

RightRoomScene::~RightRoomScene()
{
}

void RightRoomScene::onLoadResources()
{
	this->loadSceneTextures();
	this->loadAnimatedTextures();
	this->loadFonts();
	AudioManager::getInstance()->load("sounds/Music/SideRooms.wav", "SideRooms_Music");
}

void RightRoomScene::onLoadObjects()
{
	CameraManager::getInstance()->getCurrentCamera()->setPos(Vector2D(0));
	//EmptyObject* pPhysManagerHolder;
	//pPhysManagerHolder = new EmptyObject("Physics Manager Holder");
	//PhysicsManager::initialize("Physics Manager", pPhysManagerHolder);
	//GameObjectManager::getInstance()->addObject(pPhysManagerHolder);
	this->createScene();
	this->createButtons();
	this->createExitMenu();
	AudioManager::getInstance()->play(new AudioPlayer("SideRooms_Music", "BGM", AudioGroupTag::MUSIC, OnAudioFinished::LOOP));
}

void RightRoomScene::onUnloadResources()
{
	TextureManager::getInstance()->unload("Forest_Area");
	TextureManager::getInstance()->unload("Player");
	TextureManager::getInstance()->unload("Book_Yellow");
	TextureManager::getInstance()->unload("Red_Dragon");
	TextureManager::getInstance()->unload("Back");
	TextureManager::getInstance()->unload("Button");
	TextureManager::getInstance()->unload("Button_Choices");
	TextureManager::getInstance()->unload("Return_Dialogue_Holder");
	FontManager::getInstance()->unloadAllFonts();
	AudioManager::getInstance()->unload("SideRooms_Music");
}

void RightRoomScene::onUnloadObjects()
{
	AudioManager::getInstance()->stopAll();
	AScene::onUnloadObjects();
}

void RightRoomScene::loadFonts()
{
	FontManager::getInstance()->loadFont("lazy.ttf", "LazyFont90", 90);
}

void RightRoomScene::loadAnimatedTextures() 
{
	TextureManager::getInstance()->loadFromFolder("animations/player_idle", "player_idle");

}

void RightRoomScene::loadSceneTextures()
{
	std::string strPath = "right_room/forest_lobby.png";
	TextureManager::getInstance()->load(strPath, "Forest_Area");
	strPath = "right_room/book5.png";
	TextureManager::getInstance()->load(strPath, "Book_Yellow");
	strPath = "right_room/red_dragon.png";
	TextureManager::getInstance()->load(strPath, "Red_Dragon");
	TextureManager::getInstance()->load("GUI/button.png", "Button");
	TextureManager::getInstance()->load("GUI/back.png", "Back");
	TextureManager::getInstance()->load("GUI/tablet.png", "Return_Dialogue_Holder");
	TextureManager::getInstance()->load("GUI/title_button.png", "Button_Choices");
}

void RightRoomScene::createButtons()
{
	GUIButton* pButtonLeft = new GUIButton("Button_Left", "Button");
	pButtonLeft->setPos(Vector2D(-800, 0));
	pButtonLeft->setScale(Vector2D(0.25f, 0.25f));
	SceneSwitcher* pLeftRoomSwitch = new SceneSwitcher(SceneTag::LOBBY_SCENE);
	pButtonLeft->attachComponent(pLeftRoomSwitch);
	GameObjectManager::getInstance()->addObject(pButtonLeft);

	SpriteRenderer* pRenderer = (SpriteRenderer*)pButtonLeft->findComponentByName("SpriteRenderer");
	pRenderer->setFlipX(true);

	GUIButton* pReturn = new GUIButton("Return_Button", "Back");
	pReturn->setPos(Vector2D(-850, 450));
	pReturn->setScale(Vector2D(.075f, .075f));
	GUIToggle* pToggle = new GUIToggle(EventKey::RETURN_SCREEN);
	pReturn->attachComponent(pToggle);
	GameObjectManager::getInstance()->addObject(pReturn);
}

void RightRoomScene::createScene()
{
	Background* pBackground = new Background("Forest_Area", "Forest_Area", Vector2D(.65f, .9f));
	GameObjectManager::getInstance()->addObject((AGameObject*)pBackground);

	GUIButton* pBook = new GUIButton("Book_Yellow", "Book_Yellow", false);
	pBook->setPos(Vector2D(-470, 0));
	pBook->setScale(Vector2D(.5f, .5f));
	SceneSwitcher* pPuzzleRoom = new SceneSwitcher(SceneTag::PUZZLE_LEVEL_1_SCENE);
	pBook->attachComponent(pPuzzleRoom);
	GameObjectManager::getInstance()->addObject((AGameObject*)pBook);

	Sprite* pRedDragon = new Sprite("Red_Dragon", "Red_Dragon", Vector2D(-640.f, -395.f), Vector2D(.5f, .5f), 0, false);
	GameObjectManager::getInstance()->addObject((AGameObject*)pRedDragon);

	AnimatedSprite* pPlayer = new AnimatedSprite("Player", "player_idle", Vector2D(-200, -315), Vector2D(1.f, 1.f), 0.f, 8);
	GameObjectManager::getInstance()->addObject(pPlayer);
	Animation* pAnim = new Animation("idle", TextureManager::getInstance()->getTexture("player_idle"), 12, AnimationType::LOOP);
	SpriteAnimator* pSpriteAnim = (SpriteAnimator*)pPlayer->findComponentByName("SpriteAnimator");
	pSpriteAnim->addAnimation(pAnim);
	pSpriteAnim->play("idle");
}

void RightRoomScene::createExitMenu()
{
	ExitMenu* pExitMenu = new ExitMenu("LobbyExitMenu");
	GameObjectManager::getInstance()->addObject(pExitMenu);
	pExitMenu->setEnabled(false);
}