#include "LeftRoomScene.h"
#include "Background.h"
#include "Player.h"
#include "Prop.h"
#include "GUIButton.h"
#include "SceneSwitcher.h"
#include "FontManager.h"
#include "Settings.h"
#include "Text.h"
#include "GUIToggle.h"
#include "EmptyObject.h"
#include "SpriteAnimator.h"
#include "Animation.h"
#include "AnimatedSprite.h"
LeftRoomScene::LeftRoomScene():AScene(SceneTag::LEFT_ROOM_SCENE)
{
}

LeftRoomScene::~LeftRoomScene()
{
}

void LeftRoomScene::onLoadResources()
{
	this->loadSceneTextures();
	this->loadAnimatedTextures();
	this->loadFonts();
}

void LeftRoomScene::onLoadObjects()
{
	//EmptyObject* pPhysManagerHolder;
	//pPhysManagerHolder = new EmptyObject("Physics Manager Holder");
	//PhysicsManager::initialize("Physics Manager", pPhysManagerHolder);
	//GameObjectManager::getInstance()->addObject(pPhysManagerHolder);
	this->createScene();
	this->createButtons();
	this->createExitMenu();


}

void LeftRoomScene::onUnloadResources()
{
	TextureManager::getInstance()->unload("Ruins");
	TextureManager::getInstance()->unload("Button");
	TextureManager::getInstance()->unload("Pedestal");
	TextureManager::getInstance()->unload("Back");
	TextureManager::getInstance()->unload("Player");
	TextureManager::getInstance()->unload("Button_Choices");
	TextureManager::getInstance()->unload("Return_Dialogue_Holder");
	FontManager::getInstance()->unloadAllFonts();
}

void LeftRoomScene::loadFonts()
{
	FontManager::getInstance()->loadFont("lazy.ttf", "LazyFont90", 90);
}

void LeftRoomScene::loadAnimatedTextures()
{
	TextureManager::getInstance()->loadFromFolder("animations/player_idle", "player_idle");

}

void LeftRoomScene::loadSceneTextures()
{
	std::string strPath = "left_room/Ruin-background_alt.png";
	TextureManager::getInstance()->load(strPath, "Ruins");
	strPath = "left_room/pedestal.png";
	TextureManager::getInstance()->load(strPath, "Pedestal");
	TextureManager::getInstance()->load("button.png", "Button");
	TextureManager::getInstance()->load("back.png", "Back");
	TextureManager::getInstance()->load("title_screen_pngs/title_button_2.png", "Return_Dialogue_Holder");
	TextureManager::getInstance()->load("title_screen_pngs/title_button.png", "Button_Choices");
}

void LeftRoomScene::createButtons()
{
	GUIButton* pButtonRight = new GUIButton("Button_Right", "Button");
	pButtonRight->setPos(Vector2D(800, 0));
	pButtonRight->setScale(Vector2D(0.25f, 0.25f));
	SceneSwitcher* pRightRoomSwitch = new SceneSwitcher(SceneTag::LOBBY_SCENE);
	pButtonRight->attachComponent(pRightRoomSwitch);
	GameObjectManager::getInstance()->addObject(pButtonRight);

	GUIButton* pReturn = new GUIButton("Return_Button", "Back");
	pReturn->setPos(Vector2D(-850, 450));
	pReturn->setScale(Vector2D(.075f, .075f));
	GUIToggle* pToggle = new GUIToggle("Exit_Menu_BG");
	pReturn->attachComponent(pToggle);
	GameObjectManager::getInstance()->addObject(pReturn);
}

void LeftRoomScene::createScene()
{
	CameraManager::getInstance()->getCurrentCamera()->setPos(Vector2D(0));

	Background* pBackground = new Background("Ruins", "Ruins", Vector2D(1.f, .9f));
	GameObjectManager::getInstance()->addObject((AGameObject*)pBackground);
	pBackground->setPos(Vector2D(0.f, -9.f));

	Prop* pPedestal[5];
	for (int i = 0; i < 5; i++)
	{
		pPedestal[i] = new Prop("Pedestal" + i, "Pedestal", Vector2D(0, 0), Vector2D(.75f, .75f), 0.f, false);
		GameObjectManager::getInstance()->addObject((AGameObject*)pPedestal[i]);
	}
	pPedestal[0]->setPos(Vector2D(-600, -300));
	pPedestal[1]->setPos(Vector2D(-300, -300));
	pPedestal[2]->setPos(Vector2D(0, -300));
	pPedestal[3]->setPos(Vector2D(300, -300));
	pPedestal[4]->setPos(Vector2D(600, -300));

	AnimatedSprite* pPlayer = new AnimatedSprite("Player", "player_idle", Vector2D(-200, -315), Vector2D(1.f, 1.f), 0.f, 8);
	GameObjectManager::getInstance()->addObject(pPlayer);
	Animation* pAnim = new Animation("idle", TextureManager::getInstance()->getTexture("player_idle"), 12, AnimationType::LOOP);
	SpriteAnimator* pSpriteAnim = (SpriteAnimator*)pPlayer->findComponentByName("SpriteAnimator");
	pSpriteAnim->addAnimation(pAnim);
	pSpriteAnim->play("idle");
}

void LeftRoomScene::createExitMenu()
{
	Background* pExitBG = new Background("Exit_Menu_BG", "Return_Dialogue_Holder", Vector2D(.5, .5));
	pExitBG->setPos(Vector2D(575, 300));

	GameObjectManager::getInstance()->addObject(pExitBG);

	Text* pExitText = new Text("Exit_Text", "Go Back to Title \n Screen ?", "JainiPurva-Regular.ttf", 90, 0.f, false);
	pExitText->setPos(Vector2D(0, 50));
	pExitText->setScale(Vector2D(1, 1));
	pExitBG->attachChild(pExitText);


	GUIButton* pDecline = new GUIButton("Decline", "Button_Choices");
	pDecline->setPos(Vector2D(-200, -300));
	pDecline->setScale(Vector2D(.15, .15));
	GUIToggle* pToggle = new GUIToggle("Exit_Menu_BG");
	pDecline->attachComponent(pToggle);

	Text* pDeclineText = new Text("Decline_Text", "No", "JainiPurva-Regular.ttf", 90, 0.f, false);
	pDeclineText->setPos(Vector2D(-200, -300));
	pDeclineText->setScale(Vector2D(.75, .75));

	GUIButton* pAccept = new GUIButton("Accept", "Button_Choices");
	pAccept->setPos(Vector2D(250, -300));
	pAccept->setScale(Vector2D(.15, .15));
	SceneSwitcher* pTitleSwitch = new SceneSwitcher(SceneTag::TITLE_SCENE);
	pAccept->attachComponent(pTitleSwitch);
	Text* pAcceptText = new Text("Accept_Text", "Yes", "JainiPurva-Regular.ttf", 90, 0.f, false);
	pAcceptText->setPos(Vector2D(250, -300));
	pAcceptText->setScale(Vector2D(.75, .75));

	pExitBG->attachChild(pDecline);
	pDecline->attachChild(pDeclineText);

	pExitBG->attachChild(pAccept);
	pAccept->attachChild(pAcceptText);

	pExitBG->setEnabled(false);
}