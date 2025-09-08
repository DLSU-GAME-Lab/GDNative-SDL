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
	TextureManager::getInstance()->load("librarian.png", "Librarian");
	TextureManager::getInstance()->load("fairy.png", "Fairy");

	for (int i = 0; i < 16; i++)
	{
		std::string strPath = "animations/lobby_scene/player/frame" + std::to_string(i + 1) + ".png";
		TextureManager::getInstance()->load(strPath, "Player");
	}

	TextureManager::getInstance()->load("button.png", "Button");
	TextureManager::getInstance()->load("lobby_transition_lamps.png", "Lamps");
	TextureManager::getInstance()->load("stepladder_revised.png", "Step_Ladder");
	TextureManager::getInstance()->load("monoblock_revised.png", "Chair");
	TextureManager::getInstance()->load("librariandesk_revised.png", "Librarian_Desk");

}

void LobbyScene::onLoadObjects()
{
	Background* pBackground = new Background("Lobby_Background", "Lobby_Background");

	Player* pPlayer = new Player();
	Librarian* pLibrarian = new Librarian();
	Fairy* pFairy = new Fairy();

	Prop* pLamps = new Prop("Lamps", "Lamps", 0, 0, 1000, 500, false);
	Prop* pLadder = new Prop("Ladder", "Step_Ladder", 600, 430, 700, 700, false);
	Prop* pChair = new Prop("Chair", "Chair", 0, 630, 500, 500,true);
	Prop* pDesk = new Prop("Desk", "Librarian_Desk", 1000, 650, 700, 700,false);
	UIButton* pButtonRight = new UIButton("Button_Right", 1600,450, 300, 300, false);
	UIButton* pButtonLeft = new UIButton("Button_Right", 100,450, 300, 300, true);

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
