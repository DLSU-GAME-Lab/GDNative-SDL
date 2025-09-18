#include "LeftRoomScene.h"
#include "Background.h"
#include "Player.h"
#include "Prop.h"
#include "UIButton.h"

LeftRoomScene::LeftRoomScene():AScene(SceneTag::LEFT_ROOM_SCENE)
{
}

LeftRoomScene::~LeftRoomScene()
{
}

void LeftRoomScene::onLoadResources()
{
	std::string strPath = "left_room/Ruin-background_alt.png";
	TextureManager::getInstance()->load(strPath, "Ruins");
	strPath = "left_room/pedestal.png";
	TextureManager::getInstance()->load(strPath, "Pedestal");
	//strPath = "right_room/book5.png";
	//TextureManager::getInstance()->load(strPath, "Book5");
	TextureManager::getInstance()->load("button.png", "Button");
	for (int i = 0; i < 16; i++)
	{
		std::string strPath = "animations/lobby_scene/player/frame" + std::to_string(i + 1) + ".png";
		TextureManager::getInstance()->load(strPath, "Player");
	}
}

void LeftRoomScene::onLoadObjects()
{
	Background* pBackground = new Background("Ruins", "Ruins", Vector2D(1.f, .9f));
	GameObjectManager::getInstance()->addObject((AGameObject*)pBackground);
	pBackground->setPos(Vector2D(0.f, -9.f));


	UIButton* pButtonRight = new UIButton("Button_Right", "Button", Vector2D(1200, -330), Vector2D(0.25f, 0.25f), 0.0f, true, SceneTag::LOBBY_SCENE);
	GameObjectManager::getInstance()->addObject((AGameObject*)pButtonRight);

	Prop* pPedestal[5];
	for (int i = 0; i < 5; i++)
	{
		pPedestal[i] = new Prop("Pedestal" + i, "Pedestal", Vector2D(0, 0), Vector2D(.75f, .75f), 0.f, false);
		GameObjectManager::getInstance()->addObject((AGameObject*)pPedestal[i]);
	}
	pPedestal[0]->setPos(Vector2D(-600, -300));
	pPedestal[1]->setPos(Vector2D(-300, -300));
	pPedestal[2]->setPos(Vector2D(0, -300));
	pPedestal[3]->setPos(Vector2D(300, -300));
	pPedestal[4]->setPos(Vector2D(600, -300));

	Player* pPlayer = new Player(Vector2D(-160, -275), Vector2D(1.f, 1.f), 0.0f);
	GameObjectManager::getInstance()->addObject((AGameObject*)pPlayer);
}

void LeftRoomScene::onUnloadResources()
{
	TextureManager::getInstance()->unload("Ruins");
	TextureManager::getInstance()->unload("Button");
	TextureManager::getInstance()->unload("Pedestal");
	TextureManager::getInstance()->unload("Player");
}
