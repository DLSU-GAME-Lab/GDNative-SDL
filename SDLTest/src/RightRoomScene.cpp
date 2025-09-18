#include "RightRoomScene.h"
#include "Background.h"
#include "Player.h"
#include "Prop.h"
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
	strPath = "right_room/book5.png";
	TextureManager::getInstance()->load(strPath, "Book_Yellow");
	strPath = "right_room/red_dragon.png";
	TextureManager::getInstance()->load(strPath, "Red_Dragon");
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

	Prop* pBook = new Prop("Book_Yellow", "Book_Yellow", Vector2D(-470, 0), Vector2D(.5f, .5f), 0, false);
	GameObjectManager::getInstance()->addObject((AGameObject*)pBook);

	Prop* pRedDragon = new Prop("Red_Dragon", "Red_Dragon", Vector2D(-640.f, -395.f), Vector2D(.5f, .5f), 0, false);
	GameObjectManager::getInstance()->addObject((AGameObject*)pRedDragon);

	Player* pPlayer = new Player(Vector2D(0, -315), Vector2D(1.f, 1.f), 0.0f);
	GameObjectManager::getInstance()->addObject((AGameObject*)pPlayer);
}

void RightRoomScene::onUnloadResources()
{
	TextureManager::getInstance()->unload("Forest_Area");
	TextureManager::getInstance()->unload("Player");
	TextureManager::getInstance()->unload("Book_Yellow");
	TextureManager::getInstance()->unload("Red_Dragon");
}
