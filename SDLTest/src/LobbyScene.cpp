#include "LobbyScene.h"
#include "Background.h"
#include "Player.h"
#include "Librarian.h"
#include "Fairy.h"
#include "UIButton.h"
#include "Prop.h"
#include "GUIButton.h"
#include "SceneSwitcher.h"
#include "GUIToggle.h"
#include "EmptyObject.h"
#include "FontManager.h"
#include "Settings.h"
#include "Text.h"
LobbyScene::LobbyScene() : AScene(SceneTag::LOBBY_SCENE)
{

}

LobbyScene::~LobbyScene()
{

}

void LobbyScene::onLoadResources()
{
	TextureManager::getInstance()->load("library_redo.png", "Lobby_Background");

	this->loadAnimatedTextures();
	this->loadSceneTextures();
	this->loadFonts();

}

void LobbyScene::onLoadObjects()
{
	//code for when a scene needs physics
	//EmptyObject* pPhysManagerHolder;
	//pPhysManagerHolder = new EmptyObject("Physics Manager Holder");
	//PhysicsManager::initialize("Physics Manager", pPhysManagerHolder);
	//GameObjectManager::getInstance()->addObject(pPhysManagerHolder);

	this->createScene();
	GUIButton* pDiary = new GUIButton("Diary", "Diary");
	pDiary->setPos(Vector2D(0, 0));
	pDiary->setScale(Vector2D(0.25f, 0.25f));
	GUIToggle* pToggle = new GUIToggle("SWBST_BG");
	pDiary->attachComponent(pToggle);
	GameObjectManager::getInstance()->addObject(pDiary);

	Background* pSWBST_BG = new Background("SWBST_BG", "SWBST_BG", Vector2D(1, 1));
	pSWBST_BG->setEnabled(false);
	GameObjectManager::getInstance()->addObject(pSWBST_BG);
	this->createButtons();
	this->createExitMenu();



}

void LobbyScene::onUnloadResources()
{
	TextureManager::getInstance()->unload("Lobby_Background");
	TextureManager::getInstance()->unload("Player");
	TextureManager::getInstance()->unload("Librarian");
	TextureManager::getInstance()->unload("Fairy");
	TextureManager::getInstance()->unload("Button");
	TextureManager::getInstance()->unload("Lamps");
	TextureManager::getInstance()->unload("Step_Ladder");
	TextureManager::getInstance()->unload("Chair");
	TextureManager::getInstance()->unload("Librarian_Desk");
	TextureManager::getInstance()->unload("Diary");
	TextureManager::getInstance()->unload("Back");
	TextureManager::getInstance()->unload("SWBST_BG");
	TextureManager::getInstance()->unload("Start");
	TextureManager::getInstance()->unload("Button_Choices");
	TextureManager::getInstance()->unload("Return_Dialogue_Holder");
	FontManager::getInstance()->unloadFont("LazyFont");
}

void LobbyScene::loadFonts()
{
	FontManager::getInstance()->loadFont("lazy.ttf", "LazyFont90", 90);
}

void LobbyScene::loadAnimatedTextures()
{
	for (int i = 0; i < 16; i++)
	{
		std::string strPath = "animations/lobby_scene/player/frame" + std::to_string(i + 1) + ".png";
		TextureManager::getInstance()->load(strPath, "Player");
	}

	for (int i = 0; i < 19; i++)
	{
		std::string strPath = "animations/lobby_scene/fairy/frame" + std::to_string(i + 1) + ".png";
		TextureManager::getInstance()->load(strPath, "Fairy");
	}

	for (int i = 0; i < 7; i++)
	{
		std::string strPath = "animations/lobby_scene/librarian/Set" + std::to_string(i + 1) + ".png";
		TextureManager::getInstance()->load(strPath, "Librarian");
	}
}

void LobbyScene::loadSceneTextures()
{
	TextureManager::getInstance()->load("button.png", "Button");
	TextureManager::getInstance()->load("lobby_transition_lamps.png", "Lamps");
	TextureManager::getInstance()->load("stepladder_revised.png", "Step_Ladder");
	TextureManager::getInstance()->load("monoblock_revised.png", "Chair");
	TextureManager::getInstance()->load("librariandesk_revised.png", "Librarian_Desk");
	TextureManager::getInstance()->load("diary.png", "Diary");
	TextureManager::getInstance()->load("back.png", "Back");
	TextureManager::getInstance()->load("title_screen_pngs/title_button_2.png", "Return_Dialogue_Holder");
	TextureManager::getInstance()->load("title_screen_pngs/title_button.png", "Button_Choices");
	TextureManager::getInstance()->load("SWBSTWindowHolder/SWBST_BG.png", "SWBST_BG");
}

void LobbyScene::createButtons()
{
	GUIButton* pButtonRight = new GUIButton("Button_Right", "Button");
	pButtonRight->setPos(Vector2D(800, 0));
	pButtonRight->setScale(Vector2D(0.25f, 0.25f));
	SceneSwitcher* pRightRoomSwitch = new SceneSwitcher(SceneTag::RIGHT_ROOM_SCENE);
	pButtonRight->attachComponent(pRightRoomSwitch);
	GameObjectManager::getInstance()->addObject(pButtonRight);

	GUIButton* pButtonLeft = new GUIButton("Button_Left", "Button");
	pButtonLeft->setPos(Vector2D(-800, 0));
	pButtonLeft->setScale(Vector2D(0.25f, 0.25f));
	SceneSwitcher* pLeftRoomSwitch = new SceneSwitcher(SceneTag::LEFT_ROOM_SCENE);
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

void LobbyScene::createScene()
{
	Background* pBackground = new Background("Lobby_Background", "Lobby_Background", Vector2D(0.33f, 0.4f));
	GameObjectManager::getInstance()->addObject((AGameObject*)pBackground);

	Prop* pLadder = new Prop("Ladder", "Step_Ladder", Vector2D(0, -250), Vector2D(1.25f, 1.25f), 0, false);
	GameObjectManager::getInstance()->addObject((AGameObject*)pLadder);

	Player* pPlayer = new Player(Vector2D(-200, -315), Vector2D(1.f, 1.f), 0.0f);
	GameObjectManager::getInstance()->addObject((AGameObject*)pPlayer);

	Librarian* pLibrarian = new Librarian(Vector2D(450, -140), Vector2D(1.f, 1.f), 0.0f);
	GameObjectManager::getInstance()->addObject((AGameObject*)pLibrarian);

	Fairy* pFairy = new Fairy(Vector2D(250, -140), Vector2D(1.f, 1.f), 0.0f);
	GameObjectManager::getInstance()->addObject((AGameObject*)pFairy);

	Prop* pLamps = new Prop("Lamps", "Lamps", Vector2D(-550, 350), Vector2D(1.f, 1.f), 0, false);
	GameObjectManager::getInstance()->addObject((AGameObject*)pLamps);


	Prop* pChair = new Prop("Chair", "Chair", Vector2D(-600, -365), Vector2D(0.75f, 0.75f), 0, false);
	GameObjectManager::getInstance()->addObject((AGameObject*)pChair);

	Prop* pDesk = new Prop("Desk", "Librarian_Desk", Vector2D(400, -365), Vector2D(1.25f, 1.25f), 0, false);
	GameObjectManager::getInstance()->addObject((AGameObject*)pDesk);
}

void LobbyScene::createExitMenu()
{
	Background* pExitBG = new Background("Exit_Menu_BG", "Return_Dialogue_Holder", Vector2D(.5, .5));
	GameObjectManager::getInstance()->addObject(pExitBG);

	Text* pExitText = new Text("Exit_Text", "Go Back to Title", Vector2D(0, 50), Vector2D(.75, .75), 0.f, false);
	pExitText->setFont("LazyFont90");
	Text* pExitText2 = new Text("Exit_Text2", " Screen ?", Vector2D(0, -50), Vector2D(.75, .75), 0.f, false);
	pExitText2->setFont("LazyFont90");
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

	Text* pDeclineText = new Text("Decline_Text", "No", Vector2D(-200, -300), Vector2D(.75, .75), 0.f, false);
	pDeclineText->setFont("LazyFont90");
	pDecline->attachChild(pDeclineText);
	GameObjectManager::getInstance()->addObject(pDeclineText);

	GUIButton* pAccept = new GUIButton("Accept", "Button_Choices");
	pAccept->setPos(Vector2D(250, -300));
	pAccept->setScale(Vector2D(.15, .15));
	GameObjectManager::getInstance()->addObject(pAccept);
	SceneSwitcher* pTitleSwitch = new SceneSwitcher(SceneTag::TITLE_SCENE);
	pAccept->attachComponent(pTitleSwitch);
	Text* pAcceptText = new Text("Accept_Text", "Yes", Vector2D(250, -300), Vector2D(.75, .75), 0.f, false);
	pAcceptText->setFont("LazyFont90");
	pAccept->attachChild(pAcceptText);
	GameObjectManager::getInstance()->addObject(pAcceptText);

	pExitBG->attachChild(pDecline);
	pExitBG->attachChild(pAccept);

	pExitBG->setEnabled(false);
	pExitBG->setPos(Vector2D(0, 100));
}