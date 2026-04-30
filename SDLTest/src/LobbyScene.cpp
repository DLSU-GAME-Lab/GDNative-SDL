#include "LobbyScene.h"
#include "Background.h"
#include "LobbyPlayer.h"
#include "Librarian.h"
#include "Fairy.h"
#include "Sprite.h"
#include "GUIButton.h"
#include "SceneSwitcher.h"
#include "LobbyTransition.h"
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
#include "DialogueScreen.h"
#include "AudioManager.h"
LobbyScene::LobbyScene() : AScene(SceneTag::LOBBY_SCENE)
{

}

LobbyScene::~LobbyScene()
{

}

void LobbyScene::onLoadResources()
{
	this->loadAnimatedTextures();
	this->loadSceneTextures();
	this->loadFonts();
	this->loadDialogueTextures();

	AudioManager::getInstance()->load("sounds/Music/SideRooms.wav", "SideRooms_Music");
	AudioManager::getInstance()->load("sounds/Music/Lobby.wav", "Lobby_Music");

}

void LobbyScene::onLoadObjects()
{
	CameraManager::getInstance()->getCurrentCamera()->setPos(Vector2D(0));

	this->createScene();
	this->createDiary();
	this->createExitMenu();

	DialogueScreen* pDialogueScreen = new DialogueScreen("DialogueScreen");
	GameObjectManager::getInstance()->addObject(pDialogueScreen);
	pDialogueScreen->setEnabled(false);
	AudioManager::getInstance()->play(new AudioPlayer("Lobby_Music", "BGM", AudioGroupTag::MUSIC, OnAudioFinished::LOOP));
}

void LobbyScene::onUnloadResources()
{
	TextureManager::getInstance()->unload("Lobby_Background");
	TextureManager::getInstance()->unload("player_idle");
	TextureManager::getInstance()->unload("Librarian");
	TextureManager::getInstance()->unload("Fairy");
	TextureManager::getInstance()->unload("Button");
	TextureManager::getInstance()->unload("Lamps");
	TextureManager::getInstance()->unload("Bamboo");
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

	TextureManager::getInstance()->unload("Forest_Area");
	TextureManager::getInstance()->unload("Book_Yellow");
	TextureManager::getInstance()->unload("Red_Dragon");

	TextureManager::getInstance()->unload("Ruins");
	TextureManager::getInstance()->unload("Button");
	TextureManager::getInstance()->unload("Pedestal");

	TextureManager::getInstance()->unload("librarian_default");
	TextureManager::getInstance()->unload("librarian_hand");
	TextureManager::getInstance()->unload("librarian_pose");
	TextureManager::getInstance()->unload("librarian_smile");

	FontManager::getInstance()->unloadAllFonts();

	AudioManager::getInstance()->unload("Lobby_Music");
	AudioManager::getInstance()->unload("SideRooms_Music");

	EventBroadcaster::getInstance()->unregisterAllListeners();

}

void LobbyScene::onUnloadObjects()
{
	AudioManager::getInstance()->stopAll();
	AScene::onUnloadObjects();
}

void LobbyScene::loadFonts()
{
	//FontManager::getInstance()->loadFont("lazy.ttf", "lazy_12", 12);
}

void LobbyScene::loadAnimatedTextures()
{
	TextureManager::getInstance()->loadFromFolder("animations/player_idle", "player_idle");
	TextureManager::getInstance()->loadFromFolder("animations/player_run", "player_run");
	TextureManager::getInstance()->loadFromFolder("animations/lobby_scene/fairy", "Fairy");
	TextureManager::getInstance()->loadFromFolder("animations/lobby_scene/librarian", "Librarian");
}

void LobbyScene::loadSceneTextures()
{
	TextureManager::getInstance()->load("library_redo.png", "Lobby_Background");
	TextureManager::getInstance()->load("lobby_transition_lamps.png", "Lamps");
	TextureManager::getInstance()->load("lobby_transition_bamboos.png", "Bamboo");
	TextureManager::getInstance()->load("stepladder_revised.png", "Step_Ladder");
	TextureManager::getInstance()->load("monoblock_revised.png", "Chair");
	TextureManager::getInstance()->load("librariandesk_revised.png", "Librarian_Desk");
	TextureManager::getInstance()->load("diary.png", "Diary");
	TextureManager::getInstance()->load("Square.png", "Square");

	TextureManager::getInstance()->load("left_room/Ruin-background_alt.png", "Ruins");
	TextureManager::getInstance()->load("left_room/pedestal.png", "Pedestal");
	TextureManager::getInstance()->load("left_room/statue_carabao.png", "Level_1_Platformer");

	TextureManager::getInstance()->load("right_room/forest_lobby.png", "Forest_Area");
	TextureManager::getInstance()->load("right_room/book5.png", "Book_Yellow");
	TextureManager::getInstance()->load("right_room/red_dragon.png", "Red_Dragon");

	TextureManager::getInstance()->load("GUI/button.png", "Button");
	TextureManager::getInstance()->load("GUI/back.png", "Back");
	TextureManager::getInstance()->load("GUI/tablet.png", "Return_Dialogue_Holder");
	TextureManager::getInstance()->load("GUI/title_button.png", "Button_Choices");
	TextureManager::getInstance()->load("GUI/play.png", "Page_Change");
	TextureManager::getInstance()->load("GUI/play.png", "ChangeDisplay");

	TextureManager::getInstance()->load("SWBSTWindowHolder/SWBST_BG.png", "SWBST_BG");
	TextureManager::getInstance()->load("SWBSTWindowHolder/Intro.png", "Intro");
	TextureManager::getInstance()->load("SWBSTWindowHolder/Somebody.png", "Somebody");
	TextureManager::getInstance()->load("SWBSTWindowHolder/Wanted.png", "Wanted");
	TextureManager::getInstance()->load("SWBSTWindowHolder/But.png", "But");
	TextureManager::getInstance()->load("SWBSTWindowHolder/So.png", "So");
	TextureManager::getInstance()->load("SWBSTWindowHolder/Then.png", "Then");
}

void LobbyScene::loadDialogueTextures()
{
	TextureManager::getInstance()->load("GUI/library_intro/Set1.png", "librarian_default");
	TextureManager::getInstance()->load("GUI/library_intro/Set3.png", "librarian_hand");
	TextureManager::getInstance()->load("GUI/library_intro/Set4.png", "librarian_pose");
	TextureManager::getInstance()->load("GUI/library_intro/Set5.png", "librarian_smile");
}

#pragma region Objects

void LobbyScene::createScene()
{
	Sprite* pLibrary = new Sprite("Lobby_Background", "Lobby_Background", Vector2D(0.0f, 0.0f), Vector2D(0.36f, 0.36f));
	GameObjectManager::getInstance()->addObject((AGameObject*)pLibrary);

	Sprite* pLadder = new Sprite("Ladder", "Step_Ladder", Vector2D(0, -250), Vector2D(1.25f, 1.25f), 0, false);
	GameObjectManager::getInstance()->addObject((AGameObject*)pLadder);

	Librarian* pLibrarian = new Librarian(Vector2D(450, -140), Vector2D(1.f, 1.f), 0.0f);
	GameObjectManager::getInstance()->addObject((AGameObject*)pLibrarian);

	Sprite* pLamps = new Sprite("Lamps", "Lamps", Vector2D(-550, 350), Vector2D(1.f, 1.f), 0, false);
	GameObjectManager::getInstance()->addObject((AGameObject*)pLamps);

	Sprite* pChair = new Sprite("Chair", "Chair", Vector2D(-600, -365), Vector2D(0.75f, 0.75f), 0, false);
	GameObjectManager::getInstance()->addObject((AGameObject*)pChair);

	Sprite* pDesk = new Sprite("Desk", "Librarian_Desk", Vector2D(400, -365), Vector2D(1.25f, 1.25f), 0, false);
	GameObjectManager::getInstance()->addObject((AGameObject*)pDesk);
	GUIButton* pDiary = new GUIButton("DiaryButton", "Diary");
	pDiary->setPos(Vector2D(0, 0));
	pDiary->setScale(Vector2D(0.25f, 0.25f));
	GUIToggle* pDiaryToggle = new GUIToggle(EventKey::DIARY_SCREEN);
	pDiary->attachComponent(pDiaryToggle);
	Vector2D start = Vector2D(0, 0);
	Vector2D end = Vector2D(0, 50);
	TweenAnimator* pTween = new TweenAnimator();
	pTween->setAnimationType(AnimationType::YOYO);
	pTween->setTweenPos(Tween2D::from(start.x, start.y).to(end.x, end.y).during(1000).via(tweeny::easing::quadraticInOut));
	pTween->play();
	pDiary->attachComponent(pTween);

	GameObjectManager::getInstance()->addObject(pDiary);

	// Left
	Sprite* pRuins = new Sprite("Ruins", "Ruins", Vector2D(-2420.f, 20.f), Vector2D(1.0f, 1.0f));
	GameObjectManager::getInstance()->addObject((AGameObject*)pRuins);

	Sprite* pPedestal[5];
	for (int i = 0; i < 5; i++)
	{
		pPedestal[i] = new Sprite("Pedestal" + i, "Pedestal", Vector2D(0, 0), Vector2D(.75f, .75f), 0.f, false);
		GameObjectManager::getInstance()->addObject((AGameObject*)pPedestal[i]);
	}
	pPedestal[0]->setPos(Vector2D(-3020.f, -300));
	pPedestal[1]->setPos(Vector2D(-2720.f, -300));
	pPedestal[2]->setPos(Vector2D(-2420.f, -300));
	pPedestal[3]->setPos(Vector2D(-2120.f, -300));
	pPedestal[4]->setPos(Vector2D(-1820.f, -300));

	GUIButton* pLevel1Button = new GUIButton("Level1Button", "Level_1_Platformer", false);
	pLevel1Button->setPos(Vector2D(-1840, -100));
	pLevel1Button->setScale(Vector2D(.5, .5));
	SceneSwitcher* pLevel1Switcher = new SceneSwitcher(SceneTag::PLATFORMER_LEVEL_1_SCENE);
	pLevel1Button->attachComponent(pLevel1Switcher);
	GameObjectManager::getInstance()->addObject(pLevel1Button);

	// Right
	Sprite* pForest = new Sprite("Forest_Area", "Forest_Area", Vector2D(3220, 120), Vector2D(1.1f, 1.1f));
	GameObjectManager::getInstance()->addObject((AGameObject*)pForest);

	GUIButton* pBook = new GUIButton("Book_Yellow", "Book_Yellow", false);
	pBook->setPos(Vector2D(2000, 0));
	pBook->setScale(Vector2D(.5f, .5f));
	SceneSwitcher* pPuzzleRoom = new SceneSwitcher(SceneTag::PUZZLE_LEVEL_1_SCENE);
	pBook->attachComponent(pPuzzleRoom);
	GameObjectManager::getInstance()->addObject((AGameObject*)pBook);

	Sprite* pRedDragon = new Sprite("Red_Dragon", "Red_Dragon", Vector2D(1820, -395.f), Vector2D(.5f, .5f), 0, false);
	GameObjectManager::getInstance()->addObject((AGameObject*)pRedDragon);

	// Player
	LobbyPlayer* pPlayer = new LobbyPlayer();
	pPlayer->setPos(Vector2D(-200, -315));
	GameObjectManager::getInstance()->addObject(pPlayer);

	Fairy* pFairy = new Fairy(Vector2D(-120, 60), Vector2D(1.f, 1.f), 0.0f);
	pPlayer->attachChild(pFairy);

	// Bamboo
	Sprite* pBambooLeft = new Sprite("Bamboo_Left", "Bamboo", Vector2D(-1420.0f, 60.0f), Vector2D(1.2f));
	GameObjectManager::getInstance()->addObject((AGameObject*)pBambooLeft);

	Sprite* pBambooRight = new Sprite("Bamboo_Right", "Bamboo", Vector2D(1420.0f, 60.0f), Vector2D(1.2f), 0.0f, true);
	GameObjectManager::getInstance()->addObject((AGameObject*)pBambooRight);

	// GUI Buttons
	EmptyObject* pButtonHolder = new EmptyObject("Button_Holder");
	pButtonHolder->setIsScreenObject(true);
	GameObjectManager::getInstance()->addObject(pButtonHolder);

	GUIButton* pButtonRight = new GUIButton("Button_Right", "Button");
	pButtonRight->setIsScreenObject(true);
	pButtonRight->setPos(Vector2D(1760, 540));
	pButtonRight->setScale(Vector2D(0.25f, 0.25f));
	LobbyTransition* pRightRoomSwitch = new LobbyTransition(true);
	pButtonHolder->attachChild(pButtonRight);
	pButtonRight->attachComponent(pRightRoomSwitch);

	GUIButton* pButtonLeft = new GUIButton("Button_Left", "Button");
	pButtonLeft->setIsScreenObject(true);
	pButtonLeft->setPos(Vector2D(160, 540));
	pButtonLeft->setScale(Vector2D(0.25f, 0.25f));
	LobbyTransition* pLeftRoomSwitch = new LobbyTransition(false);
	pButtonHolder->attachChild(pButtonLeft);
	pButtonLeft->attachComponent(pLeftRoomSwitch);
	SpriteRenderer* pRenderer = (SpriteRenderer*)pButtonLeft->findComponentByName("SpriteRenderer");
	pRenderer->setFlipX(true);

	GUIButton* pReturn = new GUIButton("Return_Button", "Back");
	pReturn->setIsScreenObject(true);
	pReturn->setPos(Vector2D(110, 90));
	pReturn->setScale(Vector2D(.075f, .075f));
	GUIToggle* pReturnToggle = new GUIToggle(EventKey::RETURN_SCREEN);
	pReturn->attachComponent(pReturnToggle);
	pButtonHolder->attachChild(pReturn);

	pPlayer->setGUI(pButtonHolder);
	pPlayer->setLeftArrow(pButtonLeft);
	pPlayer->setRightArrow(pButtonRight);

}

void LobbyScene::createExitMenu()
{
	ExitMenu* pExitMenu = new ExitMenu("LobbyExitMenu");
	GameObjectManager::getInstance()->addObject(pExitMenu);
	pExitMenu->setEnabled(false);
}

void LobbyScene::createDiary()
{
	Diary* pDiaryGUI = new Diary("SWBST_BG", Vector2D(0, 0), Vector2D(1, 1));
	pDiaryGUI->addPageText("Ano ang SWBST?", "Ang SWBST ay isang paraan upang madaling matukoy ang mahahalagang bahagi ng \n kuwento at matulungan kang buod ng kuwento.\nAng mga titik ng SWBST ay kumakatawan sa mga sumusunod : ");
	pDiaryGUI->addPageText("Somebody", "Inilalarawan nito kung ano ang gusto ng karakter. Tanunging sa iyong sarili, ano ang \n gusto ng karakter?");
	pDiaryGUI->addPageText("Wanted", "Inilalarawan nito kung ano ang gusto ng karakter.Tanunging sa iyong sarili, ano ang gusto ng \n karakter?");
	pDiaryGUI->addPageText("But", "Kapag may karakter, may problemang kinakaharap ang karakter. Tanungin ang iyong sarili, ano \n ang problema sa kuwento na kinakaharap ng pangunahing tauhan?");
	pDiaryGUI->addPageText("So", "Sinusubukan ng pangunahing tauhan lutasin ang problema. Tanungin ang iyong sarili kung \n Paano malulutas ng karakte ang problema.");
	pDiaryGUI->addPageText("Then", "Ano ang mangyayari pagkatapos subukan ng pangunahing tauhan na lutasin ang problema? \n ");
	pDiaryGUI->addPageText("Summary", "Upang Ibuod ang mga tanong na dapat mong itanong sa iyong sarili pagkatapos \n basahin ang isang kuwento:");
	GameObjectManager::getInstance()->addObject(pDiaryGUI);

	GUIButton* pClose = new GUIButton("Close_Button", "Back");
	GUIToggle* pToggleClose = new GUIToggle(EventKey::DIARY_SCREEN);
	pClose->attachComponent(pToggleClose);
	pDiaryGUI->attachChild(pClose);
	pClose->setPos(Vector2D(-750, 400));
	pClose->setScale(Vector2D(.05f, .05f));
	//-755, -365
	GUIButton* pProgress = new GUIButton("Progress", "Page_Change");
	PageChangeToggle* pForward = new PageChangeToggle("Diary", true);
	pProgress->attachComponent(pForward);
	pDiaryGUI->attachChild(pProgress);
	pProgress->setPos(Vector2D(770, -365));
	pProgress->setScale(Vector2D(.05f, .05f));

	GUIButton* pRegress = new GUIButton("Regress", "Page_Change", true);
	PageChangeToggle* pBackward = new PageChangeToggle("Diary", false);
	pRegress->attachComponent(pBackward);
	pDiaryGUI->attachChild(pRegress);
	pRegress->setPos(Vector2D(-755, -365));
	pRegress->setScale(Vector2D(.05f, .05f));

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

	pDiaryGUI->addPage(pFirstPageHolder);
	pDiaryGUI->addPage(pSecondPageHolder);
	pDiaryGUI->addPage(pThirdPageHolder);
	pDiaryGUI->addPage(pFourthPageHolder);
	pDiaryGUI->addPage(pFifthPageHolder);
	pDiaryGUI->addPage(pSixthPageHolder);
	pDiaryGUI->addPage(pSeventhPageHolder);

	pDiaryGUI->setEnabled(false);

}

void LobbyScene::createPageOne(AGameObject* pParent)
{
	
	Sprite* pIntro = new Sprite("FairyIntro", "Intro", Vector2D(0, -0), Vector2D(1, 1), 0, false);
	pParent->attachChild(pIntro);

	Text* pS = new Text("S_Letter", "JainiPurva-Regular.ttf", 90, 0, false);
	pParent->attachChild(pS);
	pS->setMessage("S");
	pS->setColor(colorRed);
	pS->setPos(Vector2D(-580, 40));
	pS->setScale(Vector2D(1.5, 1.5));

	Text* pOmebody = new Text("omebody", "JainiPurva-Regular.ttf", 45, 0, false);
	pParent->attachChild(pOmebody);
	pOmebody->setMessage("omebody");
	pOmebody->setColor(colorRed);
	pOmebody->setPos(Vector2D(-440, 25));
	pOmebody->setScale(Vector2D(1.5, 1.5));

	Text* pW = new Text("W_Letter", "JainiPurva-Regular.ttf", 90, 0, false);
	pParent->attachChild(pW);
	pW->setMessage("W");
	pW->setColor(colorYellow);
	pW->setPos(Vector2D(-580, -140));
	pW->setScale(Vector2D(1.5, 1.5));

	Text* pAnted = new Text("anted", "JainiPurva-Regular.ttf", 45, 0, false);
	pParent->attachChild(pAnted);
	pAnted->setMessage("anted");
	pAnted->setColor(colorYellow);
	pAnted->setPos(Vector2D(-470, -160));
	pAnted->setScale(Vector2D(1.5, 1.5));

	Text* pB = new Text("B_Letter", "JainiPurva-Regular.ttf", 90, 0, false);
	pParent->attachChild(pB);
	pB->setMessage("B");
	pB->setColor(colorCyan);
	pB->setPos(Vector2D(-580, -300));
	pB->setScale(Vector2D(1.5, 1.5));

	Text* pUt = new Text("ut", "JainiPurva-Regular.ttf", 45, 0, false);
	pParent->attachChild(pUt);
	pUt->setMessage("ut");
	pUt->setColor(colorCyan);
	pUt->setPos(Vector2D(-520, -320));
	pUt->setScale(Vector2D(1.5, 1.5));

	Text* pS2 = new Text("S2_Letter", "JainiPurva-Regular.ttf", 90, 0, false);
	pParent->attachChild(pS2);
	pS2->setMessage("S");
	pS2->setColor(colorBlue);
	pS2->setPos(Vector2D(-110, 40));
	pS2->setScale(Vector2D(1.5, 1.5));

	Text* pO = new Text("O_Letter", "JainiPurva-Regular.ttf", 45, 0, false);
	pParent->attachChild(pO);
	pO->setMessage("o");
	pO->setColor(colorBlue);
	pO->setPos(Vector2D(-55, 25));
	pO->setScale(Vector2D(1.5, 1.5));

	Text* pT = new Text("T_Letter", "JainiPurva-Regular.ttf", 90, 0, false);
	pParent->attachChild(pT);
	pT->setMessage("T");
	pT->setColor(colorPurple);
	pT->setPos(Vector2D(-110, -140));
	pT->setScale(Vector2D(1.5, 1.5));

	Text* pHen = new Text("hen", "JainiPurva-Regular.ttf", 45, 0, false);
	pParent->attachChild(pHen);
	pHen->setMessage("hen");
	pHen->setColor(colorPurple);
	pHen->setPos(Vector2D(-55, -160));
	pHen->setScale(Vector2D(1.5, 1.5));
}

void LobbyScene::createPageTwo(AGameObject* pParent)
{
	Sprite* pSomebody = new Sprite("Somebody", "Somebody", Vector2D(0, -85), Vector2D(.75, .75), 0, false);
	pParent->attachChild(pSomebody);

	Text* pTao = new Text("Tao", "JainiPurva-Regular.ttf", 45, 0, false);
	pParent->attachChild(pTao);
	pTao->setMessage("Tao");
	pTao->setPos(Vector2D(-330, 0));
	pTao->setScale(Vector2D(1.5, 1.5));
	pTao->setColor(colorRed);

	Text* pHayop = new Text("Hayop", "JainiPurva-Regular.ttf", 45, 0, false);
	pParent->attachChild(pHayop);
	pHayop->setMessage("Hayop");
	pHayop->setPos(Vector2D(300, 10));
	pHayop->setScale(Vector2D(1.5, 1.5));
	pHayop->setColor(colorYellow);

	Text* pAtbp = new Text("Atbp", "JainiPurva-Regular.ttf", 45, 0, false);
	pParent->attachChild(pAtbp);
	pAtbp->setMessage("at kahit isang nagsasalitang piraso ng papel");
	pAtbp->setPos(Vector2D(360, -400));
	pAtbp->setScale(Vector2D(1, 1));
	pAtbp->setColor(colorBlue);
}

void LobbyScene::createPageThree(AGameObject* pParent)
{
	Sprite* pWanted = new Sprite("Wanted", "Wanted", Vector2D(0, -85), Vector2D(.75, .75), 0, false);
	pParent->attachChild(pWanted);

	Text* pStudent = new Text("Student", "JainiPurva-Regular.ttf", 45, 0, false);
	pParent->attachChild(pStudent);
	pStudent->setMessage("The student wanted \n to play video games");
	pStudent->setPos(Vector2D(-400, -295));
	pStudent->setScale(Vector2D(1, 1));
	pStudent->setColor(colorRed);

	Text* pCat = new Text("Cat", "JainiPurva-Regular.ttf", 45, 0, false);
	pParent->attachChild(pCat);
	pCat->setMessage("The cat wanted to \n eat a fish");
	pCat->setPos(Vector2D(0, -295));
	pCat->setScale(Vector2D(1, 1));
	pCat->setColor(colorYellow);

	Text* pFairy = new Text("Fairy", "JainiPurva-Regular.ttf", 45, 0, false);
	pParent->attachChild(pFairy);
	pFairy->setMessage("The fairy wanted to \n become a human");
	pFairy->setPos(Vector2D(400, -295));
	pFairy->setScale(Vector2D(1, 1));
	pFairy->setColor(colorBlue);

}

void LobbyScene::createPageFour(AGameObject* pParent)
{
	Sprite* pBut = new Sprite("But", "But", Vector2D(0, -85), Vector2D(.75, .75), 0, false);
	pParent->attachChild(pBut);
	
	Text* pStudent = new Text("Page4_Student", "JainiPurva-Regular.ttf", 45, 0, false);
	pParent->attachChild(pStudent);
	pStudent->setMessage("The student had a \n failing grade in \n English");
	pStudent->setPos(Vector2D(-400, -350));
	pStudent->setScale(Vector2D(1.f, 1.f));
	pStudent->setColor(colorRed);

	Text* pCat = new Text("Page4_Cat", "JainiPurva-Regular.ttf",45, 0, false);
	pParent->attachChild(pCat);
	pCat->setMessage("The cat did not have \n enough money to buy \n the fish");
	pCat->setPos(Vector2D(0, -275));
	pCat->setScale(Vector2D(1.f, 1.f));
	pCat->setColor(colorYellow);

	Text* pFairy = new Text("Page4_Fairy", "JainiPurva-Regular.ttf", 45, 0, false);
	pParent->attachChild(pFairy);
	pFairy->setMessage("The fairy couldn't \n leave her duty \n behind");
	pFairy->setPos(Vector2D(400, -275));
	pFairy->setScale(Vector2D(1.f, 1.f));
	pFairy->setColor(colorBlue);

}

void LobbyScene::createPageFive(AGameObject* pParent)
{
	Sprite* pSo = new Sprite("So", "So", Vector2D(0, -40), Vector2D(.75, .75), 0, false);
	pParent->attachChild(pSo);

	Text* pStudent = new Text("Page5_Student", "JainiPurva-Regular.ttf", 45, 0, false);
	pParent->attachChild(pStudent);
	pStudent->setMessage("The student studied \n hard.");
	pStudent->setPos(Vector2D(-400, -340));
	pStudent->setScale(Vector2D(1.f, 1.f));
	pStudent->setColor(colorRed);

	Text* pCat = new Text("Page5_Cat", "JainiPurva-Regular.ttf", 45, 0, false);
	pParent->attachChild(pCat);
	pCat->setMessage("The cat worked as a \n deliveryman to earn \n enough money.");
	pCat->setPos(Vector2D(0, -340));
	pCat->setScale(Vector2D(1.f, 1.f));
	pCat->setColor(colorYellow);

	Text* pFairy = new Text("Page5_Fairy", "JainiPurva-Regular.ttf", 45, 0, false);
	pParent->attachChild(pFairy);
	pFairy->setMessage("The fairy accompanied \n the student in his \n studies.");
	pFairy->setPos(Vector2D(400, -340));
	pFairy->setScale(Vector2D(1.f, 1.f));
	pFairy->setColor(colorBlue);
}

void LobbyScene::createPageSix(AGameObject* pParent)
{
	Sprite* pThen = new Sprite("Then", "Then", Vector2D(0, -0), Vector2D(.75, .75), 0, false);
	pParent->attachChild(pThen);

	Text* pStudent = new Text("Page6_Student", "JainiPurva-Regular.ttf", 45, 0, false);
	pParent->attachChild(pStudent);
	pStudent->setMessage("The student got a \n perfect score on his \n next quiz.");
	pStudent->setPos(Vector2D(-400, -325));
	pStudent->setScale(Vector2D(1.f, 1.f));
	pStudent->setColor(colorRed);

	Text* pCat = new Text("Page6_Cat", "JainiPurva-Regular.ttf", 45, 0, false);
	pParent->attachChild(pCat);
	pCat->setMessage("The cat was able to earn \n enough money to buy the \n fish.");
	pCat->setPos(Vector2D(0, -325));
	pCat->setScale(Vector2D(1.f, 1.f));
	pCat->setColor(colorYellow);

	Text* pFairy = new Text("Page6_Fairy", "JainiPurva-Regular.ttf",45, 0, false);
	pParent->attachChild(pFairy);
	pFairy->setMessage("The fairy became \n human.");
	pFairy->setPos(Vector2D(400, -275));
	pFairy->setScale(Vector2D(1.f, 1.f));
	pFairy->setColor(colorBlue);



}

void LobbyScene::createPageSeven(AGameObject* pParent)
{
	Text* pS = new Text("S_Letter1", "JainiPurva-Regular.ttf", 90, 0, false);
	pParent->attachChild(pS);
	pS->setMessage("S");
	pS->setPos(Vector2D(-560, 110));
	pS->setScale(Vector2D(1.f, 1.f));
	pS->setColor(colorRed);

	Text* pOmebody = new Text("omebody1", "JainiPurva-Regular.ttf", 45, 0, false);
	pParent->attachChild(pOmebody);
	pOmebody->setMessage("omebody");
	pOmebody->setPos(Vector2D(-470, 100));
	pOmebody->setScale(Vector2D(1.f, 1.f));
	pOmebody->setColor(colorRed);

	Text* pS1Line = new Text("S1_Line", "JainiPurva-Regular.ttf", 45, 0.f, false);
	pParent->attachChild(pS1Line);
	pS1Line->setMessage("Sino ang Pangunahiing tauhan?");
	pS1Line->setPos(Vector2D(-195, 100));
	pS1Line->setScale(Vector2D(.75, .75));
	pS1Line->setColor(colorRed);

	Text* pW = new Text("W_Letter1", "JainiPurva-Regular.ttf", 90, 0, false);
	pParent->attachChild(pW);
	pW->setMessage("W");
	pW->setPos(Vector2D(-560, 15));
	pW->setScale(Vector2D(1, 1));
	pW->setColor(colorYellow);

	Text* pAnted = new Text("anted1", "JainiPurva-Regular.ttf", 45, 0, false);
	pParent->attachChild(pAnted);
	pAnted->setMessage("anted");
	pAnted->setPos(Vector2D(-485, 0));
	pAnted->setScale(Vector2D(1, 1));
	pAnted->setColor(colorYellow);

	Text* pWLine = new Text("W_Line", "JainiPurva-Regular.ttf", 45, 0.f, false);
	pParent->attachChild(pWLine);
	pWLine->setMessage("Ano ang gusto ng karakter?");
	pWLine->setPos(Vector2D(-220, 0));
	pWLine->setScale(Vector2D(.75, .75));
	pWLine->setColor(colorYellow);

	Text* pB = new Text("B_Letter1", "JainiPurva-Regular.ttf", 90, 0, false);
	pParent->attachChild(pB);
	pB->setMessage("B");
	pB->setPos(Vector2D(-560, -80));
	pB->setScale(Vector2D(1, 1));
	pB->setColor(colorCyan);

	Text* pUt = new Text("ut1", "JainiPurva-Regular.ttf", 45, 0, false);
	pParent->attachChild(pUt);
	pUt->setMessage("ut");
	pUt->setPos(Vector2D(-510, -90));
	pUt->setScale(Vector2D(1, 1));
	pUt->setColor(colorCyan);

	Text* pBLine = new Text("B_Line", "JainiPurva-Regular.ttf", 45, 0.f, false);
	pParent->attachChild(pBLine);
	pBLine->setMessage("Ano ang pumipigil sa karakter sa pagkamit sa kanyang nais?");
	pBLine->setPos(Vector2D(-40, -90));
	pBLine->setScale(Vector2D(.75, .75));
	pBLine->setColor(colorCyan);

	Text* pS2 = new Text("S2_Letter1", "JainiPurva-Regular.ttf", 90, 0, false);
	pParent->attachChild(pS2);
	pS2->setMessage("S");
	pS2->setPos(Vector2D(-560, -160));
	pS2->setScale(Vector2D(1, 1));
	pS2->setColor(colorBlue);

	Text* pO = new Text("O_Letter1", "JainiPurva-Regular.ttf", 45, 0, false);
	pParent->attachChild(pO);
	pO->setMessage("O");
	pO->setPos(Vector2D(-530, -175));
	pO->setScale(Vector2D(1, 1));
	pO->setColor(colorBlue);

	Text* pS2Line = new Text("S2_Line", "JainiPurva-Regular.ttf", 45, 0.f, false);
	pParent->attachChild(pS2Line);
	pS2Line->setMessage("Ano ang pumipigil sa karakter sa pagkamit sa kanyang nais?");
	pS2Line->setPos(Vector2D(-40, -175));
	pS2Line->setScale(Vector2D(.75, .75));
	pS2Line->setColor(colorBlue);

	Text* pT = new Text("T_Letter1", "JainiPurva-Regular.ttf", 90, 0, false);
	pParent->attachChild(pT);
	pT->setMessage("T");
	pT->setPos(Vector2D(-560, -280));
	pT->setScale(Vector2D(1, 1));
	pT->setColor(colorPurple);

	Text* pHen = new Text("hen1", "JainiPurva-Regular.ttf", 45, 0, false);
	pParent->attachChild(pHen);
	pHen->setMessage("hen");
	pHen->setPos(Vector2D(-520, -295));
	pHen->setScale(Vector2D(1, 1));
	pHen->setColor(colorPurple);

	Text* pTLine = new Text("T_Line", "JainiPurva-Regular.ttf", 45, 0.f, false);
	pParent->attachChild(pTLine);
	pTLine->setMessage("Ano ang Nagyari pagkatapos sinubukan ng karakter lutasin ang problema?");
	pTLine->setPos(Vector2D(35, -295));
	pTLine->setScale(Vector2D(.75, .75));
	pTLine->setColor(colorPurple);
}

#pragma endregion
