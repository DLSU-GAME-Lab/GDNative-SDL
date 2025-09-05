#include "LobbyScene.h"
#include "Background.h"

LobbyScene::LobbyScene() : AScene(SceneTag::LOBBY_SCENE)
{

}

LobbyScene::~LobbyScene()
{

}

void LobbyScene::onLoadResources()
{
	TextureManager::getInstance()->load("library_redo.png");
}

void LobbyScene::onLoadObjects()
{
	Background* pBackground = new Background("Lobby Background", "library_redo.png");
	GameObjectManager::getInstance()->addObject((AGameObject*)pBackground);
}

void LobbyScene::onUnloadResources()
{
	TextureManager::getInstance()->unload("library_redo.png");
}
