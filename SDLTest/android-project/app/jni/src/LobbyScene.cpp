#include "LobbyScene.h"
#include "Background.h"
#include "Player.h"
#include "Librarian.h"
#include "Fairy.h"
#include "UIButton.h"
#include "Prop.h"
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

}

void LobbyScene::onLoadObjects()
{
	Background* pBackground = new Background("Lobby_Background", "Lobby_Background", 0.33f,0.4f);

	Player* pPlayer = new Player(500.f,630.f,1.f,1.f,0.0f);
	Librarian* pLibrarian = new Librarian(1000.f, 350.f, 1.f, 1.f, 0.0f);
	Fairy* pFairy = new Fairy(350, 500, 1.f, 1.f, 0.0f);

	Prop* pLamps = new Prop("Lamps", "Lamps", 0, 0, 1.f, 1.f, 0, false);
	Prop* pLadder = new Prop("Ladder", "Step_Ladder", 600.f, 530.f, 1.25f, 1.25f, 0, false);
	Prop* pChair = new Prop("Chair", "Chair", 0, 750.f, 0.75f, 0.75f, 0, true);
	Prop* pDesk = new Prop("Desk", "Librarian_Desk", 1000.f, 650.f, 1.25f, 1.25f, 0, false);
	UIButton* pButtonRight = new UIButton("Button_Right", "Button_Right", 1600.f,450.f, 1.f, 1.f, 0.0f, false, SceneTag::TITLE_SCENE);
	UIButton* pButtonLeft = new UIButton("Button_Right", "Button_Right", 100.f,450.f, 1.f, 1.f,0.0f, true, SceneTag::TITLE_SCENE);

	GameObjectManager::getInstance()->addObject((AGameObject*)pBackground);
	GameObjectManager::getInstance()->addObject((AGameObject*)pLadder);
	GameObjectManager::getInstance()->addObject((AGameObject*)pPlayer);
	GameObjectManager::getInstance()->addObject((AGameObject*)pLibrarian);
	GameObjectManager::getInstance()->addObject((AGameObject*)pFairy);
	GameObjectManager::getInstance()->addObject((AGameObject*)pButtonRight);
	GameObjectManager::getInstance()->addObject((AGameObject*)pButtonLeft);
	GameObjectManager::getInstance()->addObject((AGameObject*)pLamps);
	GameObjectManager::getInstance()->addObject((AGameObject*)pChair);
	GameObjectManager::getInstance()->addObject((AGameObject*)pDesk);
	
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
}
