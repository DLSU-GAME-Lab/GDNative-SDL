#include "RightRoomScene.h"
#include "Background.h"
#include "Player.h"

RightRoomScene::RightRoomScene():AScene(SceneTag::RIGHT_ROOM_SCENE)
{
}

RightRoomScene::~RightRoomScene()
{
}

void RightRoomScene::onLoadResources()
{
	std::string strPath = "right_room/forest_lobby.png";
	TextureManager::getInstance()->load(strPath, "Forest_Area");
	for (int i = 0; i < 16; i++)
	{
		std::string strPath = "animations/lobby_scene/player/frame" + std::to_string(i + 1) + ".png";
		TextureManager::getInstance()->load(strPath, "Player");
	}
}

void RightRoomScene::onLoadObjects()
{
	Background* pBackground = new Background("Forest_Area", "Forest_Area", Vector2D(.65f, .9f));
	GameObjectManager::getInstance()->addObject((AGameObject*)pBackground);
	Player* pPlayer = new Player(Vector2D(0, -315), Vector2D(1.f, 1.f), 0.0f);
	GameObjectManager::getInstance()->addObject((AGameObject*)pPlayer);
}

void RightRoomScene::onUnloadResources()
{
	TextureManager::getInstance()->unload("Forest_Area");
	TextureManager::getInstance()->unload("Player");
}
