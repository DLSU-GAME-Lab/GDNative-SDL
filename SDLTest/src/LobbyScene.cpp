#include "LobbyScene.h"
#include "Background.h"
#include "Player.h"
#include "Librarian.h"
#include "Fairy.h"

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
}

void LobbyScene::onLoadObjects()
{
	Background* pBackground = new Background("Lobby Background", "library_redo.png");
	Player* pPlayer = new Player();
	Librarian* pLibrarian = new Librarian();
	Fairy* pFairy = new Fairy();


	GameObjectManager::getInstance()->addObject((AGameObject*)pBackground);
	GameObjectManager::getInstance()->addObject((AGameObject*)pPlayer);
	GameObjectManager::getInstance()->addObject((AGameObject*)pLibrarian);
	GameObjectManager::getInstance()->addObject((AGameObject*)pFairy);
}

void LobbyScene::onUnloadResources()
{
	TextureManager::getInstance()->unload("library_redo.png");
	TextureManager::getInstance()->unload("frame1.png");
	TextureManager::getInstance()->unload("librarian.png");
	TextureManager::getInstance()->unload("fairy.png");
}
