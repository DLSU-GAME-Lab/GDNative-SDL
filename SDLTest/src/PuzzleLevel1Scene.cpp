#include "PuzzleLevel1Scene.h"
#include "TextureManager.h"
#include "GameObjectManager.h"
#include "CameraManager.h"
#include "Background.h"
#include "Prop.h"
#include "GUIButton.h"

PuzzleLevel1Scene::PuzzleLevel1Scene() : AScene(SceneTag::PUZZLE_LEVEL_1_SCENE)
{

}

PuzzleLevel1Scene::~PuzzleLevel1Scene()
{

}

void PuzzleLevel1Scene::onLoadResources()
{
	TextureManager::getInstance()->load("sequence_game/low_order_bg.png", "BG");

	TextureManager::getInstance()->load("sequence_game/Dragon_RedHead.png", "Dragon_Head");
	TextureManager::getInstance()->load("sequence_game/Dragon_RedTail.png", "Dragon_Tail");
	TextureManager::getInstance()->load("sequence_game/Dragon_RedTile.png", "Dragon_Tile");

	TextureManager::getInstance()->load("sequence_game/Token_Blank.png", "Token_Blank");
	TextureManager::getInstance()->load("sequence_game/Token_Horns.png", "Token_Horns");
	TextureManager::getInstance()->load("sequence_game/Token_Claws.png", "Token_Claws");
	TextureManager::getInstance()->load("sequence_game/Token_Eyes.png", "Token_Eyes");

	TextureManager::getInstance()->load("sequence_game/pause.png", "Pause_Button");
	TextureManager::getInstance()->load("sequence_game/story.png", "Story_Button");
}

void PuzzleLevel1Scene::onLoadObjects()
{
	CameraManager::getInstance()->getCurrentCamera()->setPos(Vector2D(0.0f));

	Background* pBG = new Background("Background", "BG", Vector2D(1.0f, 0.9f));
	GameObjectManager::getInstance()->addObject(pBG);

	Prop* pDragonHead = new Prop("Dragon_Head", "Dragon_Head", Vector2D(-360.0f, 50.0f), Vector2D(0.6f));
	GameObjectManager::getInstance()->addObject(pDragonHead);

	Prop* pDragonTail = new Prop("Dragon_Tail", "Dragon_Tail", Vector2D(450.0f, 50.0f), Vector2D(0.6f));
	GameObjectManager::getInstance()->addObject(pDragonTail);

	Prop* pDragonTile1 = new Prop("Dragon_Tile_1", "Dragon_Tile", Vector2D(-340.0f, 50.0f), Vector2D(0.8f));
	GameObjectManager::getInstance()->addObject(pDragonTile1);

	Prop* pDragonTile2 = new Prop("Dragon_Tile_2", "Dragon_Tile", Vector2D(0.0f, 50.0f), Vector2D(0.8f));
	GameObjectManager::getInstance()->addObject(pDragonTile2);

	Prop* pDragonTile3 = new Prop("Dragon_Tile_3", "Dragon_Tile", Vector2D(340.0f, 50.0f), Vector2D(0.8f));
	GameObjectManager::getInstance()->addObject(pDragonTile3);

	Prop* pBlank1 = new Prop("Token_Blank_1", "Token_Blank", Vector2D(-280.0f, -375.0f), Vector2D(0.45f));
	GameObjectManager::getInstance()->addObject(pBlank1);

	Prop* pBlank2 = new Prop("Token_Blank_2", "Token_Blank", Vector2D(0.0f, -375.0f), Vector2D(0.45f));
	GameObjectManager::getInstance()->addObject(pBlank2);

	Prop* pBlank3 = new Prop("Token_Blank_3", "Token_Blank", Vector2D(280.0f, -375.0f), Vector2D(0.45f));
	GameObjectManager::getInstance()->addObject(pBlank3);

	Prop* pTokenHorns = new Prop("Token_Horns", "Token_Horns", Vector2D(-280.0f, -375.0f), Vector2D(0.08f));
	GameObjectManager::getInstance()->addObject(pTokenHorns);

	Prop* pTokenClaws = new Prop("Token_Claws", "Token_Claws", Vector2D(0.0f, -375.0f), Vector2D(0.08f));
	GameObjectManager::getInstance()->addObject(pTokenClaws);

	Prop* pTokenEyes = new Prop("Token_Eyes", "Token_Eyes", Vector2D(280.0f, -375.0f), Vector2D(0.08f));
	GameObjectManager::getInstance()->addObject(pTokenEyes);

	GUIButton* pPauseButton = new GUIButton("Pause_Button", "Pause_Button");
	pPauseButton->setPos(Vector2D(800.0f, 400.0f));
	pPauseButton->setScale(Vector2D(0.08f));
	GameObjectManager::getInstance()->addObject(pPauseButton);

	GUIButton* pStoryButton = new GUIButton("Story_Button", "Story_Button");
	pStoryButton->setPos(Vector2D(600.0f, 400.0f));
	pStoryButton->setScale(Vector2D(0.08f));
	GameObjectManager::getInstance()->addObject(pStoryButton);

}

void PuzzleLevel1Scene::onUnloadResources()
{
	TextureManager::getInstance()->unload("BG");
	TextureManager::getInstance()->unload("Dragon_Head");
	TextureManager::getInstance()->unload("Dragon_Tail");
	TextureManager::getInstance()->unload("Dragon_Tile");
	TextureManager::getInstance()->unload("Token_Blank");
	TextureManager::getInstance()->unload("Token_Horns");
	TextureManager::getInstance()->unload("Token_Claws");
	TextureManager::getInstance()->unload("Token_Eyes");
	TextureManager::getInstance()->unload("Pause_Button");
	TextureManager::getInstance()->unload("Story_Button");
}
