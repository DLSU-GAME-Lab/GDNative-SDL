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
#include "TweenAnimator.h"
#include "DialogueBox.h"
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
	CameraManager::getInstance()->getCurrentCamera()->setPos(Vector2D(0));
	//code for when a scene needs physics
	PhysicsSystem::initialize();

	this->createScene();
	this->createButtons();
	this->createDiary();
	this->createExitMenu();

	DialogueBox* pDialogueBox = new DialogueBox("DialogueBox");
	GameObjectManager::getInstance()->addObject(pDialogueBox);
	pDialogueBox->setEnabled(false);

}

void LobbyScene::onUnloadResources()
{
	TextureManager::getInstance()->unload("Lobby_Background");
	TextureManager::getInstance()->unload("player_idle");
	TextureManager::getInstance()->unload("Librarian");
	TextureManager::getInstance()->unload("Fairy");
	TextureManager::getInstance()->unload("Button");
	TextureManager::getInstance()->unload("Lamps");
	TextureManager::getInstance()->unload("Square");
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
	FontManager::getInstance()->unloadAllFonts();
	EventBroadcaster::getInstance()->unregisterAllListeners();

}

void LobbyScene::loadFonts()
{
	FontManager::getInstance()->loadFont("lazy.ttf", "LazyFont90", 90);
}

void LobbyScene::loadAnimatedTextures()
{
	TextureManager::getInstance()->loadFromFolder("animations/player_idle", "player_idle");

	TextureManager::getInstance()->loadFromFolder("animations/lobby_scene/fairy", "Fairy");
	TextureManager::getInstance()->loadFromFolder("animations/lobby_scene/librarian", "Librarian");
}

void LobbyScene::loadSceneTextures()
{
	TextureManager::getInstance()->load("GUI/button.png", "Button");
	TextureManager::getInstance()->load("GUI/back.png", "Back");
	TextureManager::getInstance()->load("GUI/tablet.png", "Return_Dialogue_Holder");
	TextureManager::getInstance()->load("GUI/title_button.png", "Button_Choices");
	TextureManager::getInstance()->load("lobby_transition_lamps.png", "Lamps");
	TextureManager::getInstance()->load("stepladder_revised.png", "Step_Ladder");
	TextureManager::getInstance()->load("monoblock_revised.png", "Chair");
	TextureManager::getInstance()->load("librariandesk_revised.png", "Librarian_Desk");
	TextureManager::getInstance()->load("diary.png", "Diary");
	TextureManager::getInstance()->load("Square.png", "Square");
	TextureManager::getInstance()->load("SWBSTWindowHolder/SWBST_BG.png", "SWBST_BG");
	TextureManager::getInstance()->load("GUI/play.png", "ChangeDisplay");
	TextureManager::getInstance()->load("SWBSTWindowHolder/Intro.png", "Intro");
	TextureManager::getInstance()->load("SWBSTWindowHolder/Somebody.png", "Somebody");
	TextureManager::getInstance()->load("SWBSTWindowHolder/Wanted.png", "Wanted");
	TextureManager::getInstance()->load("SWBSTWindowHolder/But.png", "But");
	TextureManager::getInstance()->load("SWBSTWindowHolder/So.png", "So");
	TextureManager::getInstance()->load("SWBSTWindowHolder/Then.png", "Then");
	TextureManager::getInstance()->load("GUI/play.png", "Page_Change");
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
	GUIToggle* pToggle = new GUIToggle(EventKey::RETURN_SCREEN);
	pReturn->attachComponent(pToggle);
	GameObjectManager::getInstance()->addObject(pReturn);
}

void LobbyScene::createScene()
{
	Background* pBackground = new Background("Lobby_Background", "Lobby_Background", Vector2D(0.33f, 0.4f));
	GameObjectManager::getInstance()->addObject((AGameObject*)pBackground);

	Prop* pLadder = new Prop("Ladder", "Step_Ladder", Vector2D(0, -250), Vector2D(1.25f, 1.25f), 0, false);
	GameObjectManager::getInstance()->addObject((AGameObject*)pLadder);

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



}

void LobbyScene::createExitMenu()
{
	ExitMenu* pExitMenu = new ExitMenu("LobbyExitMenu");
	GameObjectManager::getInstance()->addObject(pExitMenu);
	pExitMenu->setEnabled(false);
}

void LobbyScene::createDiary()
{
	GUIButton* pDiary = new GUIButton("DiaryButton", "Diary");
	pDiary->setPos(Vector2D(0, 0));
	pDiary->setScale(Vector2D(0.25f, 0.25f));
	GUIToggle* pToggle = new GUIToggle(EventKey::DIARY_SCREEN);
	pDiary->attachComponent(pToggle);
	Vector2D start = Vector2D(0, 0);
	Vector2D end = Vector2D(0, 50);
	TweenAnimator* pTween = new TweenAnimator();
	pTween->setAnimationType(AnimationType::YOYO);
	pTween->setTweenPos(Tween2D::from(start.x, start.y).to(end.x, end.y).during(1000).via(tweeny::easing::quadraticInOut));
	pTween->play();
	pDiary->attachComponent(pTween);

	GameObjectManager::getInstance()->addObject(pDiary);

	Diary* pDiaryProper = new Diary("SWBST_BG", Vector2D(0, 0), Vector2D(1, 1));
	pDiaryProper->addPageText("Ano ang SWBST?", "Ang SWBST ay isang paraan upang madaling matukoy ang mahahalagang bahagi ng \n kuwento at matulungan kang buod ng kuwento.\nAng mga titik ng SWBST ay kumakatawan sa mga sumusunod : ");
	pDiaryProper->addPageText("Somebody", "Inilalarawan nito kung ano ang gusto ng karakter. Tanunging sa iyong sarili, ano ang \n gusto ng karakter?");
	pDiaryProper->addPageText("Wanted", "Inilalarawan nito kung ano ang gusto ng karakter.Tanunging sa iyong sarili, ano ang gusto ng \n karakter?");
	pDiaryProper->addPageText("But", "Kapag may karakter, may problemang kinakaharap ang karakter. Tanungin ang iyong sarili, ano \n ang problema sa kuwento na kinakaharap ng pangunahing tauhan?");
	pDiaryProper->addPageText("So", "Sinusubukan ng pangunahing tauhan lutasin ang problema. Tanungin ang iyong sarili kung \n Paano malulutas ng karakte ang problema.");
	pDiaryProper->addPageText("Then", "Ano ang mangyayari pagkatapos subukan ng pangunahing tauhan na lutasin ang problema? \n ");
	pDiaryProper->addPageText("Summary", "Upang Ibuod ang mga tanong na dapat mong itanong sa iyong sarili pagkatapos \n basahin ang isang kuwento:");
	GameObjectManager::getInstance()->addObject(pDiaryProper);

	GUIButton* pClose = new GUIButton("Close_Button", "Back");
	pClose->setPos(Vector2D(-750, 400));
	pClose->setScale(Vector2D(.05f, .05f));
	GUIToggle* pToggleClose = new GUIToggle(EventKey::DIARY_SCREEN);
	pClose->attachComponent(pToggleClose);
	pDiaryProper->attachChild(pClose);
	//-755, -365
	GUIButton* pProgress = new GUIButton("Progress", "Page_Change");
	pProgress->setPos(Vector2D(770, -365));
	pProgress->setScale(Vector2D(.05f, .05f));
	PageChangeToggle* pForward = new PageChangeToggle("Diary", true);
	pProgress->attachComponent(pForward);
	pDiaryProper->attachChild(pProgress);

	GUIButton* pRegress = new GUIButton("Regress", "Page_Change", true);
	pRegress->setPos(Vector2D(-755, -365));
	pRegress->setScale(Vector2D(.05f, .05f));
	PageChangeToggle* pBackward = new PageChangeToggle("Diary", false);
	pRegress->attachComponent(pBackward);
	pDiaryProper->attachChild(pRegress);


	EmptyObject* pFirstPageHolder = new EmptyObject("First_Page");
	this->createPageOne(pFirstPageHolder);
	pFirstPageHolder->setEnabled(false);

	EmptyObject* pSecondPageHolder = new EmptyObject("Second_Page");
	this->createPageTwo(pSecondPageHolder);
	pSecondPageHolder->setEnabled(false);

	EmptyObject* pThirdPageHolder = new EmptyObject("Third_Page");
	this->createPageThree(pThirdPageHolder);
	pThirdPageHolder->setEnabled(false);

	EmptyObject* pFourthPageHolder = new EmptyObject("Fourth_Page");
	this->createPageFour(pFourthPageHolder);
	pFourthPageHolder->setEnabled(false);

	EmptyObject* pFifthPageHolder = new EmptyObject("Fifth_Page");
	this->createPageFive(pFifthPageHolder);
	pFifthPageHolder->setEnabled(false);

	EmptyObject* pSixthPageHolder = new EmptyObject("Sixth_Page");
	this->createPageSix(pSixthPageHolder);
	pSixthPageHolder->setEnabled(false);

	EmptyObject* pSeventhPageHolder = new EmptyObject("Seventh_Page");
	this->createPageSeven(pSeventhPageHolder);
	pSeventhPageHolder->setEnabled(false);

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
	
	Prop* pIntro = new Prop("FairyIntro", "Intro", Vector2D(0, -0), Vector2D(1, 1), 0, false);
	pParent->attachChild(pIntro);

	Text* pS = new Text("S_Letter", "JainiPurva-Regular.ttf", 90, 0, false);
	pS->setMessage("S");
	pS->setPos(Vector2D(-580, 40)); 
	pS->setScale(Vector2D(1.5, 1.5));
	pS->setColor(colorRed);
	pParent->attachChild(pS);

	Text* pOmebody = new Text("omebody", "JainiPurva-Regular.ttf", 45, 0, false);
	pOmebody->setMessage("omebody");
	pOmebody->setPos(Vector2D(-440, 25));
	pOmebody->setScale(Vector2D(1.5, 1.5));
	pOmebody->setColor(colorRed);
	pParent->attachChild(pOmebody);

	Text* pW = new Text("W_Letter", "JainiPurva-Regular.ttf", 90, 0, false);
	pW->setMessage("W");
	pW->setPos(Vector2D(-580, -140));
	pW->setScale(Vector2D(1.5, 1.5));
	pW->setColor(colorYellow);
	pParent->attachChild(pW);

	Text* pAnted = new Text("anted", "JainiPurva-Regular.ttf", 45, 0, false);
	pAnted->setMessage("anted");
	pAnted->setPos(Vector2D(-470, -160));
	pAnted->setScale(Vector2D(1.5, 1.5));
	pAnted->setColor(colorYellow);
	pParent->attachChild(pAnted);

	Text* pB = new Text("B_Letter", "JainiPurva-Regular.ttf", 90, 0, false);
	pB->setMessage("B");
	pB->setPos(Vector2D(-580, -300));
	pB->setScale(Vector2D(1.5, 1.5));
	pB->setColor(colorCyan);
	pParent->attachChild(pB);

	Text* pUt = new Text("ut", "JainiPurva-Regular.ttf", 45, 0, false);
	pUt->setMessage("ut");
	pUt->setPos(Vector2D(-520, -320));
	pUt->setScale(Vector2D(1.5, 1.5));
	pUt->setColor(colorCyan);
	pParent->attachChild(pUt);

	Text* pS2 = new Text("S2_Letter", "JainiPurva-Regular.ttf", 90, 0, false);
	pS2->setMessage("S");
	pS2->setPos(Vector2D(-110, 40));
	pS2->setScale(Vector2D(1.5, 1.5));
	pS2->setColor(colorBlue);
	pParent->attachChild(pS2);

	Text* pO = new Text("O_Letter", "JainiPurva-Regular.ttf", 45, 0, false);
	pO->setMessage("o");
	pO->setPos(Vector2D(-55, 25));
	pO->setScale(Vector2D(1.5, 1.5));
	pO->setColor(colorBlue);
	pParent->attachChild(pO);

	Text* pT = new Text("T_Letter", "JainiPurva-Regular.ttf", 90, 0, false);
	pT->setMessage("T");
	pT->setPos(Vector2D(-110, -140));
	pT->setScale(Vector2D(1.5, 1.5));
	pT->setColor(colorPurple);
	pParent->attachChild(pT);

	Text* pHen = new Text("hen", "JainiPurva-Regular.ttf", 45, 0, false);
	pHen->setMessage("hen");
	pHen->setPos(Vector2D(-55, -160));
	pHen->setScale(Vector2D(1.5, 1.5));
	pHen->setColor(colorPurple);
	pParent->attachChild(pHen);
}

void LobbyScene::createPageTwo(AGameObject* pParent)
{
	Prop* pSomebody = new Prop("Somebody", "Somebody", Vector2D(0, -85), Vector2D(.75, .75), 0, false);
	pParent->attachChild(pSomebody);

	Text* pTao = new Text("Tao", "JainiPurva-Regular.ttf", 45, 0, false);
	pTao->setMessage("Tao");
	pTao->setPos(Vector2D(-330, 0));
	pTao->setScale(Vector2D(1.5, 1.5));
	pTao->setColor(colorRed);
	pParent->attachChild(pTao);

	Text* pHayop = new Text("Hayop", "JainiPurva-Regular.ttf", 45, 0, false);
	pHayop->setMessage("Hayop");
	pHayop->setPos(Vector2D(300, 10));
	pHayop->setScale(Vector2D(1.5, 1.5));
	pHayop->setColor(colorYellow);
	pParent->attachChild(pHayop);

	Text* pAtbp = new Text("Atbp", "JainiPurva-Regular.ttf", 45, 0, false);
	pAtbp->setMessage("at kahit isang nagsasalitang piraso ng papel");
	pAtbp->setPos(Vector2D(360, -400));
	pAtbp->setScale(Vector2D(1, 1));
	pAtbp->setColor(colorBlue);
	pParent->attachChild(pAtbp);
}

void LobbyScene::createPageThree(AGameObject* pParent)
{
	Prop* pWanted = new Prop("Wanted", "Wanted", Vector2D(0, -85), Vector2D(.75, .75), 0, false);
	pParent->attachChild(pWanted);

	Text* pStudent = new Text("Student", "JainiPurva-Regular.ttf", 45, 0, false);
	pStudent->setMessage("The student wanted \n to play video games");
	pStudent->setPos(Vector2D(-400, -295));
	pStudent->setScale(Vector2D(1, 1));
	pStudent->setColor(colorRed);
	pParent->attachChild(pStudent);

	Text* pCat = new Text("Cat", "JainiPurva-Regular.ttf", 45, 0, false);
	pCat->setMessage("The cat wanted to \n eat a fish");
	pCat->setPos(Vector2D(0, -295));
	pCat->setScale(Vector2D(1, 1));
	pCat->setColor(colorYellow);
	pParent->attachChild(pCat);

	Text* pFairy = new Text("Fairy", "JainiPurva-Regular.ttf", 45, 0, false);
	pFairy->setMessage("The fairy wanted to \n become a human");
	pFairy->setPos(Vector2D(400, -295));
	pFairy->setScale(Vector2D(1, 1));
	pFairy->setColor(colorBlue);
	pParent->attachChild(pFairy);

}

void LobbyScene::createPageFour(AGameObject* pParent)
{
	Prop* pBut = new Prop("But", "But", Vector2D(0, -85), Vector2D(.75, .75), 0, false);
	pParent->attachChild(pBut);
	
	Text* pStudent = new Text("Page4_Student", "JainiPurva-Regular.ttf", 45, 0, false);
	pStudent->setMessage("The student had a \n failing grade in \n English");
	pStudent->setPos(Vector2D(-400, -350));
	pStudent->setScale(Vector2D(1.f, 1.f));
	pStudent->setColor(colorRed);
	pParent->attachChild(pStudent);

	Text* pCat = new Text("Page4_Cat", "JainiPurva-Regular.ttf",45, 0, false);
	pCat->setMessage("The cat did not have \n enough money to buy \n the fish");
	pCat->setPos(Vector2D(0, -275));
	pCat->setScale(Vector2D(1.f, 1.f));
	pCat->setColor(colorYellow);
	pParent->attachChild(pCat);

	Text* pFairy = new Text("Page4_Fairy", "JainiPurva-Regular.ttf", 45, 0, false);
	pFairy->setMessage("The fairy couldn't \n leave her duty \n behind");
	pFairy->setPos(Vector2D(400, -275));
	pFairy->setScale(Vector2D(1.f, 1.f));
	pFairy->setColor(colorBlue);
	pParent->attachChild(pFairy);

}

void LobbyScene::createPageFive(AGameObject* pParent)
{
	Prop* pSo = new Prop("So", "So", Vector2D(0, -40), Vector2D(.75, .75), 0, false);
	pParent->attachChild(pSo);

	Text* pStudent = new Text("Page5_Student", "JainiPurva-Regular.ttf", 45, 0, false);
	pStudent->setMessage("The student studied \n hard.");
	pStudent->setPos(Vector2D(-400, -340));
	pStudent->setScale(Vector2D(1.f, 1.f));
	pStudent->setColor(colorRed);
	pParent->attachChild(pStudent);

	Text* pCat = new Text("Page5_Cat", "JainiPurva-Regular.ttf", 45, 0, false);
	pCat->setMessage("The cat worked as a \n deliveryman to earn \n enough money.");
	pCat->setPos(Vector2D(0, -340));
	pCat->setScale(Vector2D(1.f, 1.f));
	pCat->setColor(colorYellow);
	pParent->attachChild(pCat);

	Text* pFairy = new Text("Page5_Fairy", "JainiPurva-Regular.ttf", 45, 0, false);
	pFairy->setMessage("The fairy accompanied \n the student in his \n studies.");
	pFairy->setPos(Vector2D(400, -340));
	pFairy->setScale(Vector2D(1.f, 1.f));
	pFairy->setColor(colorBlue);
	pParent->attachChild(pFairy);
}

void LobbyScene::createPageSix(AGameObject* pParent)
{
	Prop* pThen = new Prop("Then", "Then", Vector2D(0, -0), Vector2D(.75, .75), 0, false);
	pParent->attachChild(pThen);

	Text* pStudent = new Text("Page6_Student", "JainiPurva-Regular.ttf", 45, 0, false);
	pStudent->setMessage("The student got a \n perfect score on his \n next quiz.");
	pStudent->setPos(Vector2D(-400, -325));
	pStudent->setScale(Vector2D(1.f, 1.f));
	pStudent->setColor(colorRed);
	pParent->attachChild(pStudent);

	Text* pCat = new Text("Page6_Cat", "JainiPurva-Regular.ttf", 45, 0, false);
	pCat->setMessage("The cat was able to earn \n enough money to buy the \n fish.");
	pCat->setPos(Vector2D(0, -325));
	pCat->setScale(Vector2D(1.f, 1.f));
	pCat->setColor(colorYellow);
	pParent->attachChild(pCat);

	Text* pFairy = new Text("Page6_Fairy", "JainiPurva-Regular.ttf",45, 0, false);
	pFairy->setMessage("The fairy became \n human.");
	pFairy->setPos(Vector2D(400, -275));
	pFairy->setScale(Vector2D(1.f, 1.f));
	pFairy->setColor(colorBlue);
	pParent->attachChild(pFairy);



}

void LobbyScene::createPageSeven(AGameObject* pParent)
{
	Text* pS = new Text("S_Letter1", "JainiPurva-Regular.ttf", 90, 0, false);
	pS->setMessage("S");
	pS->setPos(Vector2D(-560, 110));
	pS->setScale(Vector2D(1.f, 1.f));
	pS->setColor(colorRed);
	pParent->attachChild(pS);

	Text* pOmebody = new Text("omebody1", "JainiPurva-Regular.ttf", 45, 0, false);
	pOmebody->setMessage("omebody");
	pOmebody->setPos(Vector2D(-470, 100));
	pOmebody->setScale(Vector2D(1.f, 1.f));
	pOmebody->setColor(colorRed);
	pParent->attachChild(pOmebody);

	Text* pS1Line = new Text("S1_Line", "JainiPurva-Regular.ttf", 45, 0.f, false);
	pS1Line->setMessage("Sino ang Pangunahiing tauhan?");
	pS1Line->setPos(Vector2D(-195, 100));
	pS1Line->setScale(Vector2D(.75, .75));
	pS1Line->setColor(colorRed);
	pParent->attachChild(pS1Line);

	Text* pW = new Text("W_Letter1", "JainiPurva-Regular.ttf", 90, 0, false);
	pW->setMessage("W");
	pW->setPos(Vector2D(-560, 15));
	pW->setScale(Vector2D(1, 1));
	pW->setColor(colorYellow);
	pParent->attachChild(pW);

	Text* pAnted = new Text("anted1", "JainiPurva-Regular.ttf", 45, 0, false);
	pAnted->setMessage("anted");
	pAnted->setPos(Vector2D(-485, 0));
	pAnted->setScale(Vector2D(1, 1));
	pAnted->setColor(colorYellow);
	pParent->attachChild(pAnted);

	Text* pWLine = new Text("W_Line", "JainiPurva-Regular.ttf", 45, 0.f, false);
	pWLine->setMessage("Ano ang gusto ng karakter?");
	pWLine->setPos(Vector2D(-220, 0));
	pWLine->setScale(Vector2D(.75, .75));
	pWLine->setColor(colorYellow);
	pParent->attachChild(pWLine);

	Text* pB = new Text("B_Letter1", "JainiPurva-Regular.ttf", 90, 0, false);
	pB->setMessage("B");
	pB->setPos(Vector2D(-560, -80));
	pB->setScale(Vector2D(1, 1));
	pB->setColor(colorCyan);
	pParent->attachChild(pB);

	Text* pUt = new Text("ut1", "JainiPurva-Regular.ttf", 45, 0, false);
	pUt->setMessage("ut");
	pUt->setPos(Vector2D(-510, -90));
	pUt->setScale(Vector2D(1, 1));
	pUt->setColor(colorCyan);
	pParent->attachChild(pUt);

	Text* pBLine = new Text("B_Line", "JainiPurva-Regular.ttf", 45, 0.f, false);
	pBLine->setMessage("Ano ang pumipigil sa karakter sa pagkamit sa kanyang nais?");
	pBLine->setPos(Vector2D(-40, -90));
	pBLine->setScale(Vector2D(.75, .75));
	pBLine->setColor(colorCyan);
	pParent->attachChild(pBLine);

	Text* pS2 = new Text("S2_Letter1", "JainiPurva-Regular.ttf", 90, 0, false);
	pS2->setMessage("S");
	pS2->setPos(Vector2D(-560, -160));
	pS2->setScale(Vector2D(1, 1));
	pS2->setColor(colorBlue);
	pParent->attachChild(pS2);

	Text* pO = new Text("O_Letter1", "JainiPurva-Regular.ttf", 45, 0, false);
	pO->setMessage("O");
	pO->setPos(Vector2D(-530, -175));
	pO->setScale(Vector2D(1, 1));
	pO->setColor(colorBlue);
	pParent->attachChild(pO);

	Text* pS2Line = new Text("S2_Line", "JainiPurva-Regular.ttf", 45, 0.f, false);
	pS2Line->setMessage("Ano ang pumipigil sa karakter sa pagkamit sa kanyang nais?");
	pS2Line->setPos(Vector2D(-40, -175));
	pS2Line->setScale(Vector2D(.75, .75));
	pS2Line->setColor(colorBlue);
	pParent->attachChild(pS2Line);

	Text* pT = new Text("T_Letter1", "JainiPurva-Regular.ttf", 90, 0, false);
	pT->setMessage("T");
	pT->setPos(Vector2D(-560, -280));
	pT->setScale(Vector2D(1, 1));
	pT->setColor(colorPurple);
	pParent->attachChild(pT);

	Text* pHen = new Text("hen1", "JainiPurva-Regular.ttf", 45, 0, false);
	pHen->setMessage("hen");
	pHen->setPos(Vector2D(-520, -295));
	pHen->setScale(Vector2D(1, 1));
	pHen->setColor(colorPurple);
	pParent->attachChild(pHen);

	Text* pTLine = new Text("T_Line", "JainiPurva-Regular.ttf", 45, 0.f, false);
	pTLine->setMessage("Ano ang Nagyari pagkatapos sinubukan ng karakter lutasin ang problema?");
	pTLine->setPos(Vector2D(35, -295));
	pTLine->setScale(Vector2D(.75, .75));
	pTLine->setColor(colorPurple);
	pParent->attachChild(pTLine);
}

