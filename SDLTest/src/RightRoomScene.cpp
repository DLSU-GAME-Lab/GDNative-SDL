#include "RightRoomScene.h"
#include "Background.h"
#include "Player.h"
#include "Prop.h"
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
	FontManager::getInstance()->unloadFont("LazyFont90");
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
	TextureManager::getInstance()->load("button.png", "Button");
	TextureManager::getInstance()->load("back.png", "Back");
	TextureManager::getInstance()->load("title_screen_pngs/title_button_2.png", "Return_Dialogue_Holder");
	TextureManager::getInstance()->load("title_screen_pngs/title_button.png", "Button_Choices");
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
	GUIToggle* pToggle = new GUIToggle("Exit_Menu_BG");
	pReturn->attachComponent(pToggle);
	GameObjectManager::getInstance()->addObject(pReturn);
}

void RightRoomScene::createScene()
{
	Background* pBackground = new Background("Forest_Area", "Forest_Area", Vector2D(.65f, .9f));
	GameObjectManager::getInstance()->addObject((AGameObject*)pBackground);

	Prop* pBook = new Prop("Book_Yellow", "Book_Yellow", Vector2D(-470, 0), Vector2D(.5f, .5f), 0, false);
	GameObjectManager::getInstance()->addObject((AGameObject*)pBook);

	Prop* pRedDragon = new Prop("Red_Dragon", "Red_Dragon", Vector2D(-640.f, -395.f), Vector2D(.5f, .5f), 0, false);
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
	Background* pExitBG = new Background("Exit_Menu_BG", "Return_Dialogue_Holder", Vector2D(.5, .5));
	GameObjectManager::getInstance()->addObject(pExitBG);

	Text* pExitText = new Text("Exit_Text", "Go Back to Title", Vector2D(0, 50), Vector2D(.75, .75), 0.f, false);
	pExitText->setFont("LazyFont90");
	Text* pExitText2 = new Text("Exit_Text2", " Screen ?", Vector2D(0, -50), Vector2D(.75, .75), 0.f, false);
	pExitText2->setFont("LazyFont90");
	pExitBG->attachChild(pExitText);
	pExitBG->attachChild(pExitText2);


	GUIButton* pDecline = new GUIButton("Decline", "Button_Choices");
	pDecline->setPos(Vector2D(-200, -300));
	pDecline->setScale(Vector2D(.15, .15));
	GUIToggle* pToggle = new GUIToggle("Exit_Menu_BG");
	pDecline->attachComponent(pToggle);

	Text* pDeclineText = new Text("Decline_Text", "No", Vector2D(-200, -300), Vector2D(.75, .75), 0.f, false);
	pDeclineText->setFont("LazyFont90");

	GUIButton* pAccept = new GUIButton("Accept", "Button_Choices");
	pAccept->setPos(Vector2D(250, -300));
	pAccept->setScale(Vector2D(.15, .15));
	SceneSwitcher* pTitleSwitch = new SceneSwitcher(SceneTag::TITLE_SCENE);
	pAccept->attachComponent(pTitleSwitch);
	Text* pAcceptText = new Text("Accept_Text", "Yes", Vector2D(250, -300), Vector2D(.75, .75), 0.f, false);
	pAcceptText->setFont("LazyFont90");

	pExitBG->attachChild(pDecline);
	pDecline->attachChild(pDeclineText);

	pExitBG->attachChild(pAccept);
	pAccept->attachChild(pAcceptText);


	pExitBG->setEnabled(false);
	pExitBG->setPos(Vector2D(0, 100));
}
