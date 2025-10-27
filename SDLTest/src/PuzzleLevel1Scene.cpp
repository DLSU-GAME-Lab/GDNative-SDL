#include "PuzzleLevel1Scene.h"
#include "TextureManager.h"
#include "GameObjectManager.h"
#include "CameraManager.h"
#include "FontManager.h"
#include "Background.h"
#include "Prop.h"
#include "PuzzleToken.h"
#include "GUIButton.h"
#include "GUIToggle.h"
#include "Text.h"
#include "AnimatedSprite.h"
#include "SpriteAnimator.h"
#include "SequenceGameManager.h"

PuzzleLevel1Scene::PuzzleLevel1Scene() : AScene(SceneTag::PUZZLE_LEVEL_1_SCENE)
{

}

PuzzleLevel1Scene::~PuzzleLevel1Scene()
{

}

void PuzzleLevel1Scene::onLoadResources()
{
	TextureManager::getInstance()->load("Square.png", "Square");
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
	TextureManager::getInstance()->load("sequence_game/tablet.png", "Tablet");

	TextureManager::getInstance()->loadFromFolder("animations/level_intro", "Intro");

	FontManager::getInstance()->loadFont("Maragsa.otf", "Maragsa", 200);
	FontManager::getInstance()->loadFont("lazy.ttf", "LazyFont90", 90);
	FontManager::getInstance()->loadFont("JainiPurva-Regular.ttf", "Jaini90", 90);
	FontManager::getInstance()->loadFont("lazy.ttf", "LazyFont45", 45);
	FontManager::getInstance()->loadFont("JainiPurva-Regular.ttf", "Jaini45", 45);
}

void PuzzleLevel1Scene::onLoadObjects()
{
	CameraManager::getInstance()->getCurrentCamera()->setPos(Vector2D(0.0f));
	SequenceGameManager::initialize();

	Background* pBG = new Background("Background", "BG", Vector2D(1.0f, 0.9f));
	GameObjectManager::getInstance()->addObject(pBG);

	Prop* pDragonHead = new Prop("Dragon_Head", "Dragon_Head", Vector2D(-360.0f, 50.0f), Vector2D(0.6f));
	Prop* pDragonTail = new Prop("Dragon_Tail", "Dragon_Tail", Vector2D(450.0f, 50.0f), Vector2D(0.6f));

	GameObjectManager::getInstance()->addObject(pDragonHead);
	GameObjectManager::getInstance()->addObject(pDragonTail);

	Prop* pDragonTile1 = new Prop("Dragon_Tile_1", "Dragon_Tile", Vector2D(-410.0f, 50.0f), Vector2D(0.95f));
	Prop* pDragonTile2 = new Prop("Dragon_Tile_2", "Dragon_Tile", Vector2D(-30.0f, 50.0f), Vector2D(0.95f));
	Prop* pDragonTile3 = new Prop("Dragon_Tile_3", "Dragon_Tile", Vector2D(350.0f, 50.0f), Vector2D(0.95f));
	Text* pTileText1 = new Text("Tile_Text_1", "1", "Maragsa.otf", 200,0,false);
	pTileText1->setPos(Vector2D(-380.0f, 30.0f));
	Text* pTileText2 = new Text("Tile_Text_2", "2", "Maragsa.otf", 200, 0, false);
	pTileText2->setPos(Vector2D(0.0f, 30.0f));
	Text* pTileText3 = new Text("Tile_Text_3", "3", "Maragsa.otf", 200, 0, false);
	pTileText3->setPos(Vector2D(380.0f, 30.0f));

	pTileText1->setColor({ 100, 31, 31, 255 });
	pTileText2->setColor({ 100, 31, 31, 255 });
	pTileText3->setColor({ 100, 31, 31, 255 });

	GameObjectManager::getInstance()->addObject(pDragonTile1);
	GameObjectManager::getInstance()->addObject(pDragonTile2);
	GameObjectManager::getInstance()->addObject(pDragonTile3);
	GameObjectManager::getInstance()->addObject(pTileText1);
	GameObjectManager::getInstance()->addObject(pTileText2);
	GameObjectManager::getInstance()->addObject(pTileText3);

	Prop* pBlank1 = new Prop("Token_Blank_1", "Token_Blank", Vector2D(-280.0f, -375.0f), Vector2D(0.45f));
	Prop* pBlank2 = new Prop("Token_Blank_2", "Token_Blank", Vector2D(0.0f, -375.0f), Vector2D(0.45f));
	Prop* pBlank3 = new Prop("Token_Blank_3", "Token_Blank", Vector2D(280.0f, -375.0f), Vector2D(0.45f));

	GameObjectManager::getInstance()->addObject(pBlank1);
	GameObjectManager::getInstance()->addObject(pBlank2);
	GameObjectManager::getInstance()->addObject(pBlank3);

	PuzzleToken* pTokenHorns = new PuzzleToken("Token_Horns", Vector2D(-280.0f, -375.0f), Vector2D(0.08f));
	PuzzleToken* pTokenClaws = new PuzzleToken("Token_Claws", Vector2D(0.0f, -375.0f), Vector2D(0.08f));
	PuzzleToken* pTokenEyes = new PuzzleToken("Token_Eyes", Vector2D(280.0f, -375.0f), Vector2D(0.08f));

	GameObjectManager::getInstance()->addObject(pTokenHorns);
	GameObjectManager::getInstance()->addObject(pTokenClaws);
	GameObjectManager::getInstance()->addObject(pTokenEyes);

	SequenceGameManager::getInstance()->addToken(pTokenHorns);
	SequenceGameManager::getInstance()->addToken(pTokenClaws);
	SequenceGameManager::getInstance()->addToken(pTokenEyes);

	GUIButton* pPauseButton = new GUIButton("Pause_Button", "Pause_Button");
	pPauseButton->setPos(Vector2D(800.0f, 400.0f));
	pPauseButton->setScale(Vector2D(0.08f));
	//GUIToggle* pPauseToggle = new GUIToggle("Pause_Screen");
	//pPauseButton->attachComponent(pPauseToggle);

	GUIButton* pStoryButton = new GUIButton("Story_Button", "Story_Button");
	pStoryButton->setPos(Vector2D(600.0f, 400.0f));
	pStoryButton->setScale(Vector2D(0.08f));
	//GUIToggle* pStoryToggle = new GUIToggle("Story_Screen");
	//pStoryButton->attachComponent(pStoryToggle);

	GameObjectManager::getInstance()->addObject(pPauseButton);
	GameObjectManager::getInstance()->addObject(pStoryButton);

	Background* pTransBG = new Background("Transparent_BG", "Square", Vector2D(5.0f));
	GameObjectManager::getInstance()->addObject(pTransBG);
	SpriteRenderer* pTransBGR = (SpriteRenderer*)pTransBG->findComponentByName("SpriteRenderer");
	pTransBGR->setColor({0, 0, 0, 127});

	Prop* pTablet = new Prop("Tablet", "Tablet", Vector2D(0.0f), Vector2D(0.9f));
	pTransBG->attachChild(pTablet);

	/*Text* pTabletText1 = new Text("Tablet_Text_1", "Objctive:", Vector2D(-500.0f, 150.0f));
	pTabletText1->setFont("Jaini90");
	pTablet->attachChild(pTabletText1);
	SpriteRenderer* pTabletTextR1 = (SpriteRenderer*)pTabletText1->findComponentByName("SpriteRenderer");
	pTabletTextR1->setPivot(Vector2D(0.0f, 0.5f));

	Text* pTabletText2 = new Text("Tablet_Text_2", "Form the Bakunawa!", Vector2D(-500.0f, 50.0f));
	pTabletText2->setFont("Jaini90");
	pTablet->attachChild(pTabletText2);
	SpriteRenderer* pTabletTextR2 = (SpriteRenderer*)pTabletText2->findComponentByName("SpriteRenderer");
	pTabletTextR2->setPivot(Vector2D(0.0f, 0.5f));

	Text* pTabletText3 = new Text("Tablet_Text_3", "Relics to sequence: 3", Vector2D(-500.0f, -120.0f));
	pTabletText3->setFont("Jaini90");
	pTablet->attachChild(pTabletText3);
	SpriteRenderer* pTabletTextR3 = (SpriteRenderer*)pTabletText3->findComponentByName("SpriteRenderer");
	pTabletTextR3->setPivot(Vector2D(0.0f, 0.5f));*/

	pTransBG->setEnabled(false);
	//AnimatedSprite* pIntro = new AnimatedSprite("Intro", "Intro", Vector2D(100.0f, 0.0f), Vector2D(0.9f), 0.0f, 12);
	//GameObjectManager::getInstance()->addObject(pIntro);
	//SpriteAnimator* pAnimator = (SpriteAnimator*)pIntro->findComponentByName("SpriteAnimator");
	//pAnimator->getCurrentAnimation()->setType(AnimationType::ONCE);

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
	TextureManager::getInstance()->unload("Tablet");
	TextureManager::getInstance()->unload("Intro");
	FontManager::getInstance()->unloadAllFonts();
}
