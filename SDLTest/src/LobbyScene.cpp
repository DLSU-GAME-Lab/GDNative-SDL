#include "LobbyScene.h"
#include "Background.h"
#include "Player.h"
#include "Librarian.h"
#include "Fairy.h"
#include "UIButton.h"
#include "Lamps.h"
#include "Ladder.h"
#include "Chair.h"
#include "Prop.h"
LobbyScene::LobbyScene() : AScene(SceneTag::LOBBY_SCENE)
{

}

LobbyScene::~LobbyScene()
{

}

void LobbyScene::onLoadResources()
{
	TextureManager::getInstance()->load("library_redo.png");
	TextureManager::getInstance()->load("frame1.png");
	TextureManager::getInstance()->load("librarian.png");
	TextureManager::getInstance()->load("fairy.png");
	TextureManager::getInstance()->load("button.png");
	TextureManager::getInstance()->load("lobby_transition_lamps.png");
	TextureManager::getInstance()->load("stepladder_revised.png");
	TextureManager::getInstance()->load("monoblock_revised.png");
	TextureManager::getInstance()->load("librariandesk_revised.png");

}

void LobbyScene::onLoadObjects()
{
	Background* pBackground = new Background("Lobby Background", "library_redo.png");
	Player* pPlayer = new Player();
	Librarian* pLibrarian = new Librarian();
	Lamps* pLamps = new Lamps();
	Fairy* pFairy = new Fairy();
	Prop* pLadder = new Prop("Ladder", "stepladder_revised.png", 600, 430, 700, 700, false);
	Prop* pChair = new Prop("Chair", "monoblock_revised.png", 0, 630, 500, 500,true);
	Prop* pDesk = new Prop("Desk", "librariandesk_revised.png", 1000, 650, 700, 700,false);
	UIButton* pButtonRight = new UIButton("Button Right", 1600,450, 300, 300, false);
	UIButton* pButtonLeft = new UIButton("Button Right", 100,450, 300, 300, true);



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
	TextureManager::getInstance()->unload("library_redo.png");
	TextureManager::getInstance()->unload("frame1.png");
	TextureManager::getInstance()->unload("librarian.png");
	TextureManager::getInstance()->unload("fairy.png");
	TextureManager::getInstance()->unload("button.png");
	TextureManager::getInstance()->unload("lobby_transition_lamps.png");
	TextureManager::getInstance()->unload("stepladder_revised.png");
	TextureManager::getInstance()->load("monoblock_revised.png");
	TextureManager::getInstance()->load("librariandesk_revised.png");

}
