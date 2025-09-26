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
	this->loadReturnDialogue();
}

void LeftRoomScene::onLoadObjects()
{
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
	TextureManager::getInstance()->unload("Return");
	TextureManager::getInstance()->unload("Return2");
	TextureManager::getInstance()->unload("Decline");
	TextureManager::getInstance()->unload("Accept");
	FontManager::getInstance()->unloadFont("LazyFont");
}

void LeftRoomScene::loadReturnDialogue()
{
	FontManager::getInstance()->loadFont("lazy.ttf", "LazyFont", 90);
	TextureManager::getInstance()->loadFromText("Return", "LazyFont", "Go Back to Title", colorBlack);
	TextureManager::getInstance()->loadFromText("Return2", "LazyFont", " Screen ?", colorBlack);
	TextureManager::getInstance()->loadFromText("Decline", "LazyFont", "No", colorBlack);
	TextureManager::getInstance()->loadFromText("Accept", "LazyFont", "Yes", colorBlack);
}

void LeftRoomScene::loadAnimatedTextures()
{
	for (int i = 0; i < 16; i++)
	{
		std::string strPath = "animations/lobby_scene/player/frame" + std::to_string(i + 1) + ".png";
		TextureManager::getInstance()->load(strPath, "Player");
	}
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

	Player* pPlayer = new Player(Vector2D(-160, -275), Vector2D(1.f, 1.f), 0.0f);
	GameObjectManager::getInstance()->addObject((AGameObject*)pPlayer);
}

void LeftRoomScene::createExitMenu()
{
	Background* pExitBG = new Background("Exit_Menu_BG", "Return_Dialogue_Holder", Vector2D(.5, .5));
	GameObjectManager::getInstance()->addObject(pExitBG);

	Text* pExitText = new Text("Exit_Text", "Return", Vector2D(0, 50), Vector2D(.75, .75), 0.f, false);
	Text* pExitText2 = new Text("Exit_Text2", "Return2", Vector2D(0, -50), Vector2D(.75, .75), 0.f, false);
	pExitBG->attachChild(pExitText);
	pExitBG->attachChild(pExitText2);
	GameObjectManager::getInstance()->addObject(pExitText);
	GameObjectManager::getInstance()->addObject(pExitText2);

	GUIButton* pDecline = new GUIButton("Decline", "Button_Choices");
	pDecline->setPos(Vector2D(-200, -300));
	pDecline->setScale(Vector2D(.15, .15));
	GameObjectManager::getInstance()->addObject(pDecline);
	GUIToggle* pToggle = new GUIToggle("Exit_Menu_BG");
	pDecline->attachComponent(pToggle);

	Text* pDeclineText = new Text("Decline_Text", "Decline", Vector2D(-200, -300), Vector2D(.75, .75), 0.f, false);
	pDecline->attachChild(pDeclineText);
	GameObjectManager::getInstance()->addObject(pDeclineText);

	GUIButton* pAccept = new GUIButton("Accept", "Button_Choices");
	pAccept->setPos(Vector2D(250, -300));
	pAccept->setScale(Vector2D(.15, .15));
	GameObjectManager::getInstance()->addObject(pAccept);
	SceneSwitcher* pTitleSwitch = new SceneSwitcher(SceneTag::TITLE_SCENE);
	pAccept->attachComponent(pTitleSwitch);
	Text* pAcceptText = new Text("Accept_Text", "Accept", Vector2D(250, -300), Vector2D(.75, .75), 0.f, false);
	pAccept->attachChild(pAcceptText);
	GameObjectManager::getInstance()->addObject(pAcceptText);

	pExitBG->attachChild(pDecline);
	pExitBG->attachChild(pAccept);

	pExitBG->setEnabled(false);
	pExitBG->setPos(Vector2D(0, 100));
}