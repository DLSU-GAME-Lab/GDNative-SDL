#include "LobbyScene.h"
#include "Background.h"
#include "Player.h"
#include "Librarian.h"
#include "Fairy.h"
#include "Prop.h"
#include "GUIButton.h"
#include "SceneSwitcher.h"
#include "GUIToggle.h"
#include "EmptyObject.h"
#include "Settings.h"
#include "Text.h"
#include "Diary.h"
#include "DiaryToggle.h"
#include "PageChangeToggle.h"
#include "Platform.h"
#include "AnimatedSprite.h"
#include "Animation.h"
#include "SpriteAnimator.h"
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
	EmptyObject* pPhysManagerHolder;
	pPhysManagerHolder = new EmptyObject("Physics Manager Holder");
	PhysicsManager::initialize("Physics Manager", pPhysManagerHolder);
	GameObjectManager::getInstance()->addObject(pPhysManagerHolder);

	this->createScene();
	this->createButtons();
	this->createDiary();
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
	TextureManager::getInstance()->unload("ChangeDisplay");
	TextureManager::getInstance()->unload("Intro");
	TextureManager::getInstance()->unload("Somebody");
	TextureManager::getInstance()->unload("Wanted");
	TextureManager::getInstance()->unload("But");
	TextureManager::getInstance()->unload("So");
	TextureManager::getInstance()->unload("Then");
	TextureManager::getInstance()->unload("Page_Change");
	FontManager::getInstance()->unloadFont("LazyFont90");
	FontManager::getInstance()->unloadFont("Jaini90");
	FontManager::getInstance()->unloadFont("LazyFont45");
	FontManager::getInstance()->unloadFont("Jaini45");
}

void LobbyScene::loadFonts()
{
	FontManager::getInstance()->loadFont("lazy.ttf", "LazyFont90", 90);
	FontManager::getInstance()->loadFont("JainiPurva-Regular.ttf", "Jaini90", 90);
	FontManager::getInstance()->loadFont("lazy.ttf", "LazyFont45", 45);
	FontManager::getInstance()->loadFont("JainiPurva-Regular.ttf", "Jaini45", 45);
}

void LobbyScene::loadAnimatedTextures()
{
	TextureManager::getInstance()->loadFromFolder("animations/player_idle", "player_idle");
	TextureManager::getInstance()->loadFromFolder("animations/player_run", "player_run");
	TextureManager::getInstance()->loadFromFolder("animations/player_jump", "player_jump");

	TextureManager::getInstance()->loadFromFolder("animations/lobby_scene/fairy", "Fairy");
	TextureManager::getInstance()->loadFromFolder("animations/lobby_scene/librarian", "Librarian");
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
	TextureManager::getInstance()->load("SWBSTWindowHolder/Play_Icon.png", "ChangeDisplay");
	TextureManager::getInstance()->load("SWBSTWindowHolder/Intro.png", "Intro");
	TextureManager::getInstance()->load("SWBSTWindowHolder/Somebody.png", "Somebody");
	TextureManager::getInstance()->load("SWBSTWindowHolder/Wanted.png", "Wanted");
	TextureManager::getInstance()->load("SWBSTWindowHolder/But.png", "But");
	TextureManager::getInstance()->load("SWBSTWindowHolder/So.png", "So");
	TextureManager::getInstance()->load("SWBSTWindowHolder/Then.png", "Then");
	TextureManager::getInstance()->load("SWBSTWindowHolder/Play_Icon.png", "Page_Change");
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

	//Player* pPlayer = new Player(Vector2D(-200, -315), Vector2D(1.f, 1.f), 0.0f);
	//GameObjectManager::getInstance()->addObject((AGameObject*)pPlayer);

	AnimatedSprite* pPlayer = new AnimatedSprite("Player", "player_idle", Vector2D(-200, -315), Vector2D(1.f, 1.f), 0.f, 8);
	GameObjectManager::getInstance()->addObject(pPlayer);
	Animation * pAnim = new Animation("idle", TextureManager::getInstance()->getTexture("player_idle"), 12, AnimationType::LOOP);
	SpriteAnimator* pSpriteAnim = (SpriteAnimator*)pPlayer->findComponentByName("SpriteAnimator");
	pSpriteAnim->addAnimation(pAnim);
	pSpriteAnim->play("idle");

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

	//Platform* pPlat = new Platform("Walk_Plat", "Button_Choices", Vector2D(-400, 200), Vector2D(.15, .15), 0.f);
	//GameObjectManager::getInstance()->addObject((AGameObject*)pPlat);

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

void LobbyScene::createDiary()
{
	GUIButton* pDiary = new GUIButton("Diary", "Diary");
	pDiary->setPos(Vector2D(0, 0));
	pDiary->setScale(Vector2D(0.25f, 0.25f));
	GUIToggle* pToggle = new GUIToggle("Diary");
	pDiary->attachComponent(pToggle);
	GameObjectManager::getInstance()->addObject(pDiary);

	Diary* pDiaryProper = new Diary("SWBST_BG", Vector2D(0, 0), Vector2D(1, 1));
	GameObjectManager::getInstance()->addObject(pDiaryProper);

	GUIButton* pClose = new GUIButton("Close_Button", "Back");
	pClose->setPos(Vector2D(-750, 400));
	pClose->setScale(Vector2D(.05f, .05f));
	DiaryToggle* pToggleClose = new DiaryToggle("Diary");
	pClose->attachComponent(pToggleClose);
	GameObjectManager::getInstance()->addObject(pClose);
	pDiaryProper->attachChild(pClose);
	//-755, -365
	GUIButton* pProgress = new GUIButton("Progress", "Page_Change");
	pProgress->setPos(Vector2D(770, -365));
	pProgress->setScale(Vector2D(.05f, .05f));
	PageChangeToggle* pForward = new PageChangeToggle("Diary", true);
	pProgress->attachComponent(pForward);
	GameObjectManager::getInstance()->addObject(pProgress);
	pDiaryProper->attachChild(pProgress);

	GUIButton* pRegress = new GUIButton("Regress", "Page_Change", true);
	pRegress->setPos(Vector2D(-755, -365));
	pRegress->setScale(Vector2D(.05f, .05f));
	PageChangeToggle* pBackward = new PageChangeToggle("Diary", false);
	pProgress->attachComponent(pBackward);
	GameObjectManager::getInstance()->addObject(pRegress);
	pDiaryProper->attachChild(pRegress);


	EmptyObject* pFirstPageHolder = new EmptyObject("First_Page");
	GameObjectManager::getInstance()->addObject(pFirstPageHolder);
	this->createPageOne(pFirstPageHolder);
	pFirstPageHolder->setEnabled(false);
	pFirstPageHolder->setFollowParent(true);

	EmptyObject* pSecondPageHolder = new EmptyObject("Second_Page");
	GameObjectManager::getInstance()->addObject(pSecondPageHolder);
	this->createPageTwo(pSecondPageHolder);
	pSecondPageHolder->setEnabled(false);
	pSecondPageHolder->setFollowParent(false);

	EmptyObject* pThirdPageHolder = new EmptyObject("Third_Page");
	GameObjectManager::getInstance()->addObject(pThirdPageHolder);
	this->createPageThree(pThirdPageHolder);
	pThirdPageHolder->setEnabled(false);
	pThirdPageHolder->setFollowParent(false);

	EmptyObject* pFourthPageHolder = new EmptyObject("Fourth_Page");
	GameObjectManager::getInstance()->addObject(pFourthPageHolder);
	this->createPageFour(pFourthPageHolder);
	pFourthPageHolder->setEnabled(false);
	pFourthPageHolder->setFollowParent(false);

	EmptyObject* pFifthPageHolder = new EmptyObject("Fifth_Page");
	GameObjectManager::getInstance()->addObject(pFifthPageHolder);
	this->createPageFive(pFifthPageHolder);
	pFifthPageHolder->setEnabled(false);
	pFifthPageHolder->setFollowParent(false);

	EmptyObject* pSixthPageHolder = new EmptyObject("Sixth_Page");
	GameObjectManager::getInstance()->addObject(pSixthPageHolder);
	this->createPageSix(pSixthPageHolder);
	pSixthPageHolder->setEnabled(false);
	pSixthPageHolder->setFollowParent(false);

	EmptyObject* pSeventhPageHolder = new EmptyObject("Seventh_Page");
	GameObjectManager::getInstance()->addObject(pSeventhPageHolder);
	this->createPageSeven(pSeventhPageHolder);
	pSeventhPageHolder->setEnabled(false);
	pSeventhPageHolder->setFollowParent(false);

	pDiaryProper->addPage(pFirstPageHolder);
	pDiaryProper->addPage(pSecondPageHolder);
	pDiaryProper->addPage(pThirdPageHolder);
	pDiaryProper->addPage(pFourthPageHolder);
	pDiaryProper->addPage(pFifthPageHolder);
	pDiaryProper->addPage(pSixthPageHolder);
	pDiaryProper->addPage(pSeventhPageHolder);

	pDiaryProper->setEnabled(false);

}

void LobbyScene::createPageOne(AGameObject* pParent)
{
	
	Text* pTitle = new Text("Page1_Title", "Ano ang SWBST?", Vector2D(0, 400), Vector2D(1, 1), 0.f, false);
	pTitle->setFont("Jaini90");
	pParent->attachChild(pTitle);
	GameObjectManager::getInstance()->addObject(pTitle);

	Text* pLine1 = new Text("Page1_Line1", "Ang SWBST ay isang paraan upang madaling amtukoy ang mahahalagang bahagi ng", Vector2D(0, 300), Vector2D(.75, .75), 0.f, false);
	pLine1->setFont("Jaini45");
	pParent->attachChild(pLine1);
	GameObjectManager::getInstance()->addObject(pLine1);

	Text* pLine2 = new Text("Page1_Line2", "kuwento at matulungan kang buod ng kuwento.", Vector2D(-275, 250), Vector2D(.75, .75), 0.f, false);
	pLine2->setFont("Jaini45");
	pParent->attachChild(pLine2);
	GameObjectManager::getInstance()->addObject(pLine2);

	Text* pLine3 = new Text("Page1_Line3", "Ang mga titik ng SWBST ay kumakatawan sa mga sumusunod:", Vector2D(-180, 150), Vector2D(.75, .75), 0.f, false);
	pLine3->setFont("Jaini45");
	pParent->attachChild(pLine3);
	GameObjectManager::getInstance()->addObject(pLine3);

	Prop* pIntro = new Prop("FairyIntro", "Intro", Vector2D(0, -0), Vector2D(1, 1), 0, false);
	pParent->attachChild(pIntro);
	GameObjectManager::getInstance()->addObject((AGameObject*)pIntro);

	Text* pS = new Text("S_Letter", "S", Vector2D(-580, 40), Vector2D(1.5, 1.5), 0, false);
	pS->setColor(colorRed);
	pS->setFont("Jaini90");
	pParent->attachChild(pS);
	GameObjectManager::getInstance()->addObject((AGameObject*)pS);

	Text* pOmebody = new Text("omebody", "omebody", Vector2D(-440, 25), Vector2D(1.5, 1.5), 0, false);
	pOmebody->setColor(colorRed);
	pOmebody->setFont("Jaini45");
	pParent->attachChild(pOmebody);
	GameObjectManager::getInstance()->addObject((AGameObject*)pOmebody);

	Text* pW = new Text("W_Letter", "W", Vector2D(-580, -140), Vector2D(1.5, 1.5), 0, false);
	pW->setColor(colorYellow);
	pW->setFont("Jaini90");
	pParent->attachChild(pW);
	GameObjectManager::getInstance()->addObject((AGameObject*)pW);

	Text* pAnted = new Text("anted", "anted", Vector2D(-470, -160), Vector2D(1.5, 1.5), 0, false);
	pAnted->setColor(colorYellow);
	pAnted->setFont("Jaini45");
	pParent->attachChild(pAnted);
	GameObjectManager::getInstance()->addObject((AGameObject*)pAnted);

	Text* pB = new Text("B_Letter", "B", Vector2D(-580, -300), Vector2D(1.5, 1.5), 0, false);
	pB->setColor(colorCyan);
	pB->setFont("Jaini90");
	pParent->attachChild(pB);
	GameObjectManager::getInstance()->addObject((AGameObject*)pB);

	Text* pUt = new Text("ut", "ut", Vector2D(-520, -320), Vector2D(1.5, 1.5), 0, false);
	pUt->setColor(colorCyan);
	pUt->setFont("Jaini45");
	pParent->attachChild(pUt);
	GameObjectManager::getInstance()->addObject((AGameObject*)pUt);

	Text* pS2 = new Text("S2_Letter", "S", Vector2D(-110, 40), Vector2D(1.5, 1.5), 0, false);
	pS2->setColor(colorBlue);
	pS2->setFont("Jaini90");
	pParent->attachChild(pS2);
	GameObjectManager::getInstance()->addObject((AGameObject*)pS2);

	Text* pO = new Text("O_Letter", "o", Vector2D(-55, 25), Vector2D(1.5, 1.5), 0, false);
	pO->setColor(colorBlue);
	pO->setFont("Jaini45");
	pParent->attachChild(pO);
	GameObjectManager::getInstance()->addObject((AGameObject*)pO);

	Text* pT= new Text("T_Letter", "T", Vector2D(-110, -140), Vector2D(1.5, 1.5), 0, false);
	pT->setColor(colorPurple);
	pT->setFont("Jaini90");
	pParent->attachChild(pT);
	GameObjectManager::getInstance()->addObject((AGameObject*)pT);

	Text* pHen = new Text("hen", "hen", Vector2D(-55, -160), Vector2D(1.5, 1.5), 0, false);
	pHen->setColor(colorPurple);
	pHen->setFont("Jaini45");
	pParent->attachChild(pHen);
	GameObjectManager::getInstance()->addObject((AGameObject*)pHen);
}

void LobbyScene::createPageTwo(AGameObject* pParent)
{
	Text* pTitle = new Text("Page2_Title", "Somebody", Vector2D(0, 400), Vector2D(1, 1), 0.f, false);
	pTitle->setFont("Jaini90");
	pParent->attachChild(pTitle);
	GameObjectManager::getInstance()->addObject(pTitle);

	Text* pLine1 = new Text("Page2_Line1", "Inilalarawan nito kung ano ang gusto ng karakter. Tanunging sa iyong sarili, ano ang", Vector2D(0, 300), Vector2D(.75, .75), 0.f, false);
	pLine1->setFont("Jaini45");
	pParent->attachChild(pLine1);
	GameObjectManager::getInstance()->addObject(pLine1);

	Text* pLine2 = new Text("Page2_Line2", "gusto ng karakter?", Vector2D(-0, 250), Vector2D(.75, .75), 0.f, false);
	pLine2->setFont("Jaini45");
	pParent->attachChild(pLine2);
	GameObjectManager::getInstance()->addObject(pLine2);

	Prop* pSomebody = new Prop("Somebody", "Somebody", Vector2D(0, -85), Vector2D(.75, .75), 0, false);
	pParent->attachChild(pSomebody);
	GameObjectManager::getInstance()->addObject((AGameObject*)pSomebody);

	Text* pTao = new Text("Tao", "Tao", Vector2D(-330, 0), Vector2D(1.5, 1.5), 0, false);
	pTao->setColor(colorRed);
	pTao->setFont("Jaini45");
	pParent->attachChild(pTao);
	GameObjectManager::getInstance()->addObject((AGameObject*)pTao);

	Text* pHayop = new Text("Hayop", "Hayop", Vector2D(300, 10), Vector2D(1.5, 1.5), 0, false);
	pHayop->setColor(colorYellow);
	pHayop->setFont("Jaini45");
	pParent->attachChild(pHayop);
	GameObjectManager::getInstance()->addObject((AGameObject*)pHayop);

	Text* pAtbp = new Text("Atbp", "at kahit isang nagsasalitang piraso ng papel", Vector2D(410, -350), Vector2D(1.f, 1.f), 0, false);
	pAtbp->setColor(colorBlue);
	pAtbp->setFont("Jaini45");
	pParent->attachChild(pAtbp);
	GameObjectManager::getInstance()->addObject((AGameObject*)pAtbp);
}

void LobbyScene::createPageThree(AGameObject* pParent)
{
	Text* pTitle = new Text("Page3_Title", "Wanted", Vector2D(0, 400), Vector2D(1, 1), 0.f, false);
	pTitle->setFont("Jaini90");
	pParent->attachChild(pTitle);
	GameObjectManager::getInstance()->addObject(pTitle);

	Text* pLine1 = new Text("Page3_Line1", "Inilalarawan nito kung ano ang gusto ng karakter. Tanunging sa iyong sarili, ano ang gusto ng", Vector2D(0, 300), Vector2D(.75, .75), 0.f, false);
	pLine1->setFont("Jaini45");
	pParent->attachChild(pLine1);
	GameObjectManager::getInstance()->addObject(pLine1);

	Text* pLine2 = new Text("Page3_Line2", "karakter?", Vector2D(-0, 250), Vector2D(.75, .75), 0.f, false);
	pLine2->setFont("Jaini45");
	pParent->attachChild(pLine2);
	GameObjectManager::getInstance()->addObject(pLine2);

	Prop* pWanted = new Prop("Wanted", "Wanted", Vector2D(0, -85), Vector2D(.75, .75), 0, false);
	pParent->attachChild(pWanted);
	GameObjectManager::getInstance()->addObject((AGameObject*)pWanted);

	Text* pStudent = new Text("Student", "The student wanted", Vector2D(-400, -275), Vector2D(1.f, 1.f), 0, false);
	pStudent->setColor(colorRed);
	pStudent->setFont("Jaini45");
	pParent->attachChild(pStudent);
	GameObjectManager::getInstance()->addObject((AGameObject*)pStudent);

	Text* pStudent1 = new Text("Student1", "to play video games", Vector2D(-400, -320), Vector2D(1.f, 1.f), 0, false);
	pStudent1->setColor(colorRed);
	pStudent1->setFont("Jaini45");
	pParent->attachChild(pStudent1);
	GameObjectManager::getInstance()->addObject((AGameObject*)pStudent1);

	Text* pCat = new Text("Cat", "The cat wanted to", Vector2D(0, -275), Vector2D(1.f, 1.f), 0, false);
	pCat->setColor(colorYellow);
	pCat->setFont("Jaini45");
	pParent->attachChild(pCat);
	GameObjectManager::getInstance()->addObject((AGameObject*)pCat);

	Text* pCat1 = new Text("Cat1", "eat a fish", Vector2D(0, -320), Vector2D(1.f, 1.f), 0, false);
	pCat1->setColor(colorYellow);
	pCat1->setFont("Jaini45");
	pParent->attachChild(pCat1);
	GameObjectManager::getInstance()->addObject((AGameObject*)pCat1);

	Text* pFairy = new Text("Fairy", "The fairy wanted to", Vector2D(400, -275), Vector2D(1.f, 1.f), 0, false);
	pFairy->setColor(colorBlue);
	pFairy->setFont("Jaini45");
	pParent->attachChild(pFairy);
	GameObjectManager::getInstance()->addObject((AGameObject*)pFairy);

	Text* pFairy1 = new Text("Fairy1", "become a human", Vector2D(400, -320), Vector2D(1.f, 1.f), 0, false);
	pFairy1->setColor(colorBlue);
	pFairy1->setFont("Jaini45");
	pParent->attachChild(pFairy1);
	GameObjectManager::getInstance()->addObject((AGameObject*)pFairy1);

}

void LobbyScene::createPageFour(AGameObject* pParent)
{
	Text* pTitle = new Text("Page4_Title", "But", Vector2D(0, 400), Vector2D(1, 1), 0.f, false);
	pTitle->setFont("Jaini90");
	pParent->attachChild(pTitle);
	GameObjectManager::getInstance()->addObject(pTitle);

	Text* pLine1 = new Text("Page4_Line1", "Kapag may karakter, may problemang kinakaharap ang karakter. Tanungin ang iyong sarili, ano", Vector2D(0, 300), Vector2D(.75, .75), 0.f, false);
	pLine1->setFont("Jaini45");
	pParent->attachChild(pLine1);
	GameObjectManager::getInstance()->addObject(pLine1);

	Text* pLine2 = new Text("Page4_Line2", "ang problema sa kuwento na kinakaharap ng pangunahing tauhan?", Vector2D(-0, 250), Vector2D(.75, .75), 0.f, false);
	pLine2->setFont("Jaini45");
	pParent->attachChild(pLine2);
	GameObjectManager::getInstance()->addObject(pLine2);

	Prop* pBut = new Prop("But", "But", Vector2D(0, -85), Vector2D(.75, .75), 0, false);
	pParent->attachChild(pBut);
	GameObjectManager::getInstance()->addObject((AGameObject*)pBut);
	
	Text* pStudent = new Text("Page4_Student", "The student had a", Vector2D(-400, -275), Vector2D(1.f, 1.f), 0, false);
	pStudent->setColor(colorRed);
	pStudent->setFont("Jaini45");
	pParent->attachChild(pStudent);
	GameObjectManager::getInstance()->addObject((AGameObject*)pStudent);

	Text* pStudent1 = new Text("Page4_Student1", "failing grade in ", Vector2D(-400, -320), Vector2D(1.f, 1.f), 0, false);
	pStudent1->setColor(colorRed);
	pStudent1->setFont("Jaini45");
	pParent->attachChild(pStudent1);
	GameObjectManager::getInstance()->addObject((AGameObject*)pStudent1);

	Text* pStudent2 = new Text("Page4_Student2", "English ", Vector2D(-400, -365), Vector2D(1.f, 1.f), 0, false);
	pStudent2->setColor(colorRed);
	pStudent2->setFont("Jaini45");
	pParent->attachChild(pStudent2);
	GameObjectManager::getInstance()->addObject((AGameObject*)pStudent2);

	Text* pCat = new Text("Page4_Cat", "The cat did not have", Vector2D(0, -275), Vector2D(1.f, 1.f), 0, false);
	pCat->setColor(colorYellow);
	pCat->setFont("Jaini45");
	pParent->attachChild(pCat);
	GameObjectManager::getInstance()->addObject((AGameObject*)pCat);

	Text* pCat1 = new Text("Page4_Cat1", "enough money to buy", Vector2D(0, -320), Vector2D(1.f, 1.f), 0, false);
	pCat1->setColor(colorYellow);
	pCat1->setFont("Jaini45");
	pParent->attachChild(pCat1);
	GameObjectManager::getInstance()->addObject((AGameObject*)pCat1);

	Text* pCat2 = new Text("Page4_Cat2", "the fish", Vector2D(0, -365), Vector2D(1.f, 1.f), 0, false);
	pCat2->setColor(colorYellow);
	pCat2->setFont("Jaini45");
	pParent->attachChild(pCat2);
	GameObjectManager::getInstance()->addObject((AGameObject*)pCat2);

	Text* pFairy = new Text("Page4_Fairy", "The fairy couldn't", Vector2D(400, -275), Vector2D(1.f, 1.f), 0, false);
	pFairy->setColor(colorBlue);
	pFairy->setFont("Jaini45");
	pParent->attachChild(pFairy);
	GameObjectManager::getInstance()->addObject((AGameObject*)pFairy);

	Text* pFairy1 = new Text("Page4_Fairy1", "leave her duty", Vector2D(400, -320), Vector2D(1.f, 1.f), 0, false);
	pFairy1->setColor(colorBlue);
	pFairy1->setFont("Jaini45");
	pParent->attachChild(pFairy1);
	GameObjectManager::getInstance()->addObject((AGameObject*)pFairy1);

	Text* pFairy2 = new Text("Page4_Fairy2", "behind", Vector2D(400, -365), Vector2D(1.f, 1.f), 0, false);
	pFairy2->setColor(colorBlue);
	pFairy2->setFont("Jaini45");
	pParent->attachChild(pFairy2);
	GameObjectManager::getInstance()->addObject((AGameObject*)pFairy2);
}

void LobbyScene::createPageFive(AGameObject* pParent)
{
	Text* pTitle = new Text("Page5_Title", "So", Vector2D(0, 400), Vector2D(1, 1), 0.f, false);
	pTitle->setFont("Jaini90");
	pParent->attachChild(pTitle);
	GameObjectManager::getInstance()->addObject(pTitle);

	Text* pLine1 = new Text("Page5_Line1", "Sinusubukan ng pangunahing tauhan lutasin ang problema. Tanungin ang iyong sarili kung", Vector2D(0, 300), Vector2D(.75, .75), 0.f, false);
	pLine1->setFont("Jaini45");
	pParent->attachChild(pLine1);
	GameObjectManager::getInstance()->addObject(pLine1);

	Text* pLine2 = new Text("Page5_Line2", "Paano malulutas ng karakte ang problema.", Vector2D(-0, 250), Vector2D(.75, .75), 0.f, false);
	pLine2->setFont("Jaini45");
	pParent->attachChild(pLine2);
	GameObjectManager::getInstance()->addObject(pLine2);

	Prop* pSo = new Prop("So", "So", Vector2D(0, -40), Vector2D(.75, .75), 0, false);
	pParent->attachChild(pSo);
	GameObjectManager::getInstance()->addObject((AGameObject*)pSo);

	Text* pStudent = new Text("Page5_Student", "The student studied", Vector2D(-400, -275), Vector2D(1.f, 1.f), 0, false);
	pStudent->setColor(colorRed);
	pStudent->setFont("Jaini45");
	pParent->attachChild(pStudent);
	GameObjectManager::getInstance()->addObject((AGameObject*)pStudent);

	Text* pStudent1 = new Text("Page5_Student1", "hard.", Vector2D(-400, -320), Vector2D(1.f, 1.f), 0, false);
	pStudent1->setColor(colorRed);
	pStudent1->setFont("Jaini45");
	pParent->attachChild(pStudent1);
	GameObjectManager::getInstance()->addObject((AGameObject*)pStudent1);

	Text* pCat = new Text("Page5_Cat", "The cat worked as a", Vector2D(0, -275), Vector2D(1.f, 1.f), 0, false);
	pCat->setColor(colorYellow);
	pCat->setFont("Jaini45");
	pParent->attachChild(pCat);
	GameObjectManager::getInstance()->addObject((AGameObject*)pCat);

	Text* pCat1 = new Text("Page5_Cat1", "deliveryman to earn", Vector2D(0, -320), Vector2D(1.f, 1.f), 0, false);
	pCat1->setColor(colorYellow);
	pCat1->setFont("Jaini45");
	pParent->attachChild(pCat1);
	GameObjectManager::getInstance()->addObject((AGameObject*)pCat1);

	Text* pCat2 = new Text("Page5_Cat2", "enough money.", Vector2D(0, -365), Vector2D(1.f, 1.f), 0, false);
	pCat2->setColor(colorYellow);
	pCat2->setFont("Jaini45");
	pParent->attachChild(pCat2);
	GameObjectManager::getInstance()->addObject((AGameObject*)pCat2);

	Text* pFairy = new Text("Page5_Fairy", "The fairy accompanied", Vector2D(400, -275), Vector2D(1.f, 1.f), 0, false);
	pFairy->setColor(colorBlue);
	pFairy->setFont("Jaini45");
	pParent->attachChild(pFairy);
	GameObjectManager::getInstance()->addObject((AGameObject*)pFairy);

	Text* pFairy1 = new Text("Page5_Fairy1", "the student in his", Vector2D(400, -320), Vector2D(1.f, 1.f), 0, false);
	pFairy1->setColor(colorBlue);
	pFairy1->setFont("Jaini45");
	pParent->attachChild(pFairy1);
	GameObjectManager::getInstance()->addObject((AGameObject*)pFairy1);

	Text* pFairy2 = new Text("Page5_Fairy2", "studies.", Vector2D(400, -365), Vector2D(1.f, 1.f), 0, false);
	pFairy2->setColor(colorBlue);
	pFairy2->setFont("Jaini45");
	pParent->attachChild(pFairy2);
	GameObjectManager::getInstance()->addObject((AGameObject*)pFairy2);
}

void LobbyScene::createPageSix(AGameObject* pParent)
{
	Text* pTitle = new Text("Page6_Title", "Then", Vector2D(0, 400), Vector2D(1, 1), 0.f, false);
	pTitle->setFont("Jaini90");
	pParent->attachChild(pTitle);
	GameObjectManager::getInstance()->addObject(pTitle);

	Text* pLine1 = new Text("Page6_Line1", "Ano ang mangyayari pagkatapos subukan ng pangunahing tauhan na lutasin ang problema?", Vector2D(0, 300), Vector2D(.75, .75), 0.f, false);
	pLine1->setFont("Jaini45");
	pParent->attachChild(pLine1);
	GameObjectManager::getInstance()->addObject(pLine1);

	Prop* pThen = new Prop("Then", "Then", Vector2D(0, -0), Vector2D(.75, .75), 0, false);
	pParent->attachChild(pThen);
	GameObjectManager::getInstance()->addObject((AGameObject*)pThen);

	Text* pStudent = new Text("Page6_Student", "The student got a", Vector2D(-400, -275), Vector2D(1.f, 1.f), 0, false);
	pStudent->setColor(colorRed);
	pStudent->setFont("Jaini45");
	pParent->attachChild(pStudent);
	GameObjectManager::getInstance()->addObject((AGameObject*)pStudent);

	Text* pStudent1 = new Text("Page6_Student1", "perfect score on his", Vector2D(-400, -320), Vector2D(1.f, 1.f), 0, false);
	pStudent1->setColor(colorRed);
	pStudent1->setFont("Jaini45");
	pParent->attachChild(pStudent1);
	GameObjectManager::getInstance()->addObject((AGameObject*)pStudent1);

	Text* pStudent2 = new Text("Page6_Student2", "next quiz.", Vector2D(-400, -365), Vector2D(1.f, 1.f), 0, false);
	pStudent2->setColor(colorRed);
	pStudent2->setFont("Jaini45");
	pParent->attachChild(pStudent2);
	GameObjectManager::getInstance()->addObject((AGameObject*)pStudent2);

	Text* pCat = new Text("Page6_Cat", "The cat was able to earn", Vector2D(0, -275), Vector2D(1.f, 1.f), 0, false);
	pCat->setColor(colorYellow);
	pCat->setFont("Jaini45");
	pParent->attachChild(pCat);
	GameObjectManager::getInstance()->addObject((AGameObject*)pCat);

	Text* pCat1 = new Text("Page6_Cat1", "enough money to buy the", Vector2D(0, -320), Vector2D(1.f, 1.f), 0, false);
	pCat1->setColor(colorYellow);
	pCat1->setFont("Jaini45");
	pParent->attachChild(pCat1);
	GameObjectManager::getInstance()->addObject((AGameObject*)pCat1);

	Text* pCat2 = new Text("Page6_Cat2", "fish.", Vector2D(0, -365), Vector2D(1.f, 1.f), 0, false);
	pCat2->setColor(colorYellow);
	pCat2->setFont("Jaini45");
	pParent->attachChild(pCat2);
	GameObjectManager::getInstance()->addObject((AGameObject*)pCat2);

	Text* pFairy = new Text("Page6_Fairy", "The fairy became", Vector2D(400, -275), Vector2D(1.f, 1.f), 0, false);
	pFairy->setColor(colorBlue);
	pFairy->setFont("Jaini45");
	pParent->attachChild(pFairy);
	GameObjectManager::getInstance()->addObject((AGameObject*)pFairy);

	Text* pFairy1 = new Text("Page6_Fairy1", "human.", Vector2D(400, -320), Vector2D(1.f, 1.f), 0, false);
	pFairy1->setColor(colorBlue);
	pFairy1->setFont("Jaini45");
	pParent->attachChild(pFairy1);
	GameObjectManager::getInstance()->addObject((AGameObject*)pFairy1);


}

void LobbyScene::createPageSeven(AGameObject* pParent)
{
	Text* pTitle = new Text("Page7_Title", "Summary", Vector2D(0, 400), Vector2D(1, 1), 0.f, false);
	pTitle->setFont("Jaini90");
	pParent->attachChild(pTitle);
	GameObjectManager::getInstance()->addObject(pTitle);

	Text* pLine1 = new Text("Page7_Line1", "Upang Ibuod ang mga tanong na dapat mong itanong sa iyong sarili pagkatapos", Vector2D(0, 300), Vector2D(.75, .75), 0.f, false);
	pLine1->setFont("Jaini45");
	pParent->attachChild(pLine1);
	GameObjectManager::getInstance()->addObject(pLine1);

	Text* pLine2 = new Text("Page7_Line2", "basahin ang isang kuwento:", Vector2D(-295, 250), Vector2D(.75, .75), 0.f, false);
	pLine2->setFont("Jaini45");
	pParent->attachChild(pLine2);
	GameObjectManager::getInstance()->addObject(pLine2);

	Text* pS = new Text("S_Letter1", "S", Vector2D(-560, 110), Vector2D(1, 1), 0, false);
	pS->setColor(colorRed);
	pS->setFont("Jaini90");
	pParent->attachChild(pS);
	GameObjectManager::getInstance()->addObject((AGameObject*)pS);

	Text* pOmebody = new Text("omebody1", "omebody", Vector2D(-470, 100), Vector2D(1, 1), 0, false);
	pOmebody->setColor(colorRed);
	pOmebody->setFont("Jaini45");
	pParent->attachChild(pOmebody);
	GameObjectManager::getInstance()->addObject((AGameObject*)pOmebody);

	Text* pS1Line = new Text("S1_Line", "Sino ang Pangunahiing tauhan?", Vector2D(-195, 100), Vector2D(.75, .75), 0.f, false);
	pS1Line->setColor(colorRed);
	pS1Line->setFont("Jaini45");
	pParent->attachChild(pS1Line);
	GameObjectManager::getInstance()->addObject(pS1Line);

	Text* pW = new Text("W_Letter1", "W", Vector2D(-560, 15), Vector2D(1, 1), 0, false);
	pW->setColor(colorYellow);
	pW->setFont("Jaini90");
	pParent->attachChild(pW);
	GameObjectManager::getInstance()->addObject((AGameObject*)pW);

	Text* pAnted = new Text("anted1", "anted", Vector2D(-485, 0), Vector2D(1, 1), 0, false);
	pAnted->setColor(colorYellow);
	pAnted->setFont("Jaini45");
	pParent->attachChild(pAnted);
	GameObjectManager::getInstance()->addObject((AGameObject*)pAnted);

	Text* pWLine = new Text("W_Line", "Ano ang gusto ng karakter?", Vector2D(-220, 0), Vector2D(.75, .75), 0.f, false);
	pWLine->setColor(colorYellow);
	pWLine->setFont("Jaini45");
	pParent->attachChild(pWLine);
	GameObjectManager::getInstance()->addObject(pWLine);

	Text* pB = new Text("B_Letter1", "B", Vector2D(-560, -80), Vector2D(1, 1), 0, false);
	pB->setColor(colorCyan);
	pB->setFont("Jaini90");
	pParent->attachChild(pB);
	GameObjectManager::getInstance()->addObject((AGameObject*)pB);

	Text* pUt = new Text("ut1", "ut", Vector2D(-510, -90), Vector2D(1, 1), 0, false);
	pUt->setColor(colorCyan);
	pUt->setFont("Jaini45");
	pParent->attachChild(pUt);
	GameObjectManager::getInstance()->addObject((AGameObject*)pUt);

	Text* pBLine = new Text("B_Line", "Ano ang pumipigil sa karakter sa pagkamit sa kanyang nais?", Vector2D(-40, -90), Vector2D(.75, .75), 0.f, false);
	pBLine->setColor(colorCyan);
	pBLine->setFont("Jaini45");
	pParent->attachChild(pBLine);
	GameObjectManager::getInstance()->addObject(pBLine);

	Text* pS2 = new Text("S2_Letter1", "S", Vector2D(-560, -160), Vector2D(1, 1), 0, false);
	pS2->setColor(colorBlue);
	pS2->setFont("Jaini90");
	pParent->attachChild(pS2);
	GameObjectManager::getInstance()->addObject((AGameObject*)pS2);

	Text* pO = new Text("O_Letter1", "o", Vector2D(-530, -175), Vector2D(1, 1), 0, false);
	pO->setColor(colorBlue);
	pO->setFont("Jaini45");
	pParent->attachChild(pO);
	GameObjectManager::getInstance()->addObject((AGameObject*)pO);

	Text* pS2Line = new Text("S2_Line", "Ano ang pumipigil sa karakter sa pagkamit sa kanyang nais?", Vector2D(-40, -175), Vector2D(.75, .75), 0.f, false);
	pS2Line->setColor(colorBlue);
	pS2Line->setFont("Jaini45");
	pParent->attachChild(pS2Line);
	GameObjectManager::getInstance()->addObject(pS2Line);

	Text* pT = new Text("T_Letter1", "T", Vector2D(-560, -280), Vector2D(1, 1), 0, false);
	pT->setColor(colorPurple);
	pT->setFont("Jaini90");
	pParent->attachChild(pT);
	GameObjectManager::getInstance()->addObject((AGameObject*)pT);

	Text* pHen = new Text("hen1", "hen", Vector2D(-520, -295), Vector2D(1, 1), 0, false);
	pHen->setColor(colorPurple);
	pHen->setFont("Jaini45");
	pParent->attachChild(pHen);
	GameObjectManager::getInstance()->addObject((AGameObject*)pHen);

	Text* pTLine = new Text("T_Line", "Ano ang Nagyari pagkatapos sinubukan ng karakter lutasin ang problema?", Vector2D(35, -295), Vector2D(.75, .75), 0.f, false);
	pTLine->setColor(colorPurple);
	pTLine->setFont("Jaini45");
	pParent->attachChild(pTLine);
	GameObjectManager::getInstance()->addObject(pTLine);
}

