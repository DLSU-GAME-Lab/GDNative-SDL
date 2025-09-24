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

LobbyScene::LobbyScene() : AScene(SceneTag::LOBBY_SCENE)
{

}

LobbyScene::~LobbyScene()
{

}

void LobbyScene::onLoadResources()
{
	TextureManager::getInstance()->load("library_redo.png", "Lobby_Background");
	//TextureManager::getInstance()->load("frame1.png", "Player");
	//TextureManager::getInstance()->load("fairy.png", "Fairy");
	//TextureManager::getInstance()->load("librarian.png", "Librarian");

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

	TextureManager::getInstance()->load("button.png", "Button");
	TextureManager::getInstance()->load("lobby_transition_lamps.png", "Lamps");
	TextureManager::getInstance()->load("stepladder_revised.png", "Step_Ladder");
	TextureManager::getInstance()->load("monoblock_revised.png", "Chair");
	TextureManager::getInstance()->load("librariandesk_revised.png", "Librarian_Desk");
	TextureManager::getInstance()->load("diary.png", "Diary");

	TextureManager::getInstance()->load("SWBSTWindowHolder/SWBST_BG.png", "SWBST_BG");

}

void LobbyScene::onLoadObjects()
{
	Background* pBackground = new Background("Lobby_Background", "Lobby_Background", Vector2D(0.33f, 0.4f));
	GameObjectManager::getInstance()->addObject((AGameObject*)pBackground);

	Prop* pLadder = new Prop("Ladder", "Step_Ladder", Vector2D(0, -250), Vector2D(1.25f, 1.25f), 0, false);
	GameObjectManager::getInstance()->addObject((AGameObject*)pLadder);

	Player* pPlayer = new Player(Vector2D(-200, -315), Vector2D(1.f, 1.f), 0.0f);
	GameObjectManager::getInstance()->addObject((AGameObject*)pPlayer);

	Librarian* pLibrarian = new Librarian(Vector2D(450, -140), Vector2D(1.f, 1.f), 0.0f);
	GameObjectManager::getInstance()->addObject((AGameObject*)pLibrarian);

	Fairy* pFairy = new Fairy(Vector2D(250, -140),Vector2D(1.f,1.f), 0.0f);
	GameObjectManager::getInstance()->addObject((AGameObject*)pFairy);

	Prop* pLamps = new Prop("Lamps", "Lamps", Vector2D(-550, 350), Vector2D(1.f, 1.f), 0, false);
	GameObjectManager::getInstance()->addObject((AGameObject*)pLamps);


	Prop* pChair = new Prop("Chair", "Chair", Vector2D(-600,-365), Vector2D(0.75f, 0.75f), 0, false);
	GameObjectManager::getInstance()->addObject((AGameObject*)pChair);

	Prop* pDesk = new Prop("Desk", "Librarian_Desk", Vector2D(400, -365), Vector2D(1.25f, 1.25f), 0, false);
	GameObjectManager::getInstance()->addObject((AGameObject*)pDesk);


	GUIButton* pDiary = new GUIButton("Diary", "Diary");
	pDiary->setPos(Vector2D(0, 0));
	pDiary->setScale(Vector2D(0.25f, 0.25f));
	GUIToggle* pToggle = new GUIToggle("SWBST_BG");
	pDiary->attachComponent(pToggle);
	GameObjectManager::getInstance()->addObject(pDiary);

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

	Background* pSWBST_BG = new Background("SWBST_BG", "SWBST_BG", Vector2D(1, 1));
	pSWBST_BG->setEnabled(false);
	GameObjectManager::getInstance()->addObject(pSWBST_BG);
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
	TextureManager::getInstance()->unload("SWBST_BG");
}
