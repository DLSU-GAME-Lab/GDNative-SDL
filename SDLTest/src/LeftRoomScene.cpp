#include "LeftRoomScene.h"
#include "Background.h"
#include "Player.h"
#include "Prop.h"
#include "GUIButton.h"
#include "SceneSwitcher.h"

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
	TextureManager::getInstance()->load("back.png", "Back");
}

void LeftRoomScene::onLoadObjects()
{
	Background* pBackground = new Background("Ruins", "Ruins", Vector2D(1.f, .9f));
	GameObjectManager::getInstance()->addObject((AGameObject*)pBackground);
	pBackground->setPos(Vector2D(0.f, -9.f));

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

	GUIButton* pButtonRight = new GUIButton("Button_Right", "Button");
	pButtonRight->setPos(Vector2D(800, 0));
	pButtonRight->setScale(Vector2D(0.25f, 0.25f));
	SceneSwitcher* pRightRoomSwitch = new SceneSwitcher(SceneTag::LOBBY_SCENE);
	pButtonRight->attachComponent(pRightRoomSwitch);
	GameObjectManager::getInstance()->addObject(pButtonRight);

	GUIButton* pReturn = new GUIButton("Return_Button", "Back");
	pReturn->setPos(Vector2D(-850, 450));
	pReturn->setScale(Vector2D(.075f, .075f));
	SceneSwitcher* pTitleSwitch = new SceneSwitcher(SceneTag::TITLE_SCENE);
	pReturn->attachComponent(pTitleSwitch);
	GameObjectManager::getInstance()->addObject(pReturn);
}

void LeftRoomScene::onUnloadResources()
{
	TextureManager::getInstance()->unload("Ruins");
	TextureManager::getInstance()->unload("Button");
	TextureManager::getInstance()->unload("Pedestal");
	TextureManager::getInstance()->unload("Back");
	TextureManager::getInstance()->unload("Player");
}
