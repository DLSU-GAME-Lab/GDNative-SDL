#include "PuzzleLevel1Scene.h"
#include "TextureManager.h"
#include "GameObjectManager.h"
#include "CameraManager.h"
#include "FontManager.h"
#include "Background.h"
#include "Sprite.h"
#include "PuzzleToken.h"
#include "GUIButton.h"
#include "GUIToggle.h"
#include "Text.h"
#include "AnimatedSprite.h"
#include "SpriteAnimator.h"
#include "SequenceGameManager.h"
#include "PuzzleHint.h"
#include "ObjectiveIntro.h"
#include "EmptyObject.h"
#include "PuzzleEnd.h"
PuzzleLevel1Scene::PuzzleLevel1Scene() : AScene(SceneTag::PUZZLE_LEVEL_1_SCENE)
{

}

PuzzleLevel1Scene::~PuzzleLevel1Scene()
{

}

void PuzzleLevel1Scene::onLoadResources()
{
	TextureManager::getInstance()->load("Square.png", "Square");
	TextureManager::getInstance()->load("GUI/button6.png", "Close_Button");
	TextureManager::getInstance()->load("sequence_game/low_order_bg.png", "BG");
	

	TextureManager::getInstance()->load("sequence_game/Dragon_RedHead.png", "Dragon_Head");
	TextureManager::getInstance()->load("sequence_game/Dragon_RedTail.png", "Dragon_Tail");
	TextureManager::getInstance()->load("sequence_game/Dragon_RedTile.png", "Dragon_Tile");
	TextureManager::getInstance()->load("sequence_game/Dragon_RedFull.png", "Full_Dragon");

	TextureManager::getInstance()->load("sequence_game/Token_Blank.png", "Token_Blank");
	TextureManager::getInstance()->load("sequence_game/Token_Horns.png", "Token_Horns");
	TextureManager::getInstance()->load("sequence_game/Token_Claws.png", "Token_Claws");
	TextureManager::getInstance()->load("sequence_game/Token_Eyes.png", "Token_Eyes");

	TextureManager::getInstance()->load("GUI/pause.png", "Pause_Button");
	TextureManager::getInstance()->load("GUI/story.png", "Story_Button");
	TextureManager::getInstance()->load("GUI/tablet.png", "Tablet");

	TextureManager::getInstance()->loadFromFolder("animations/level_intro", "Intro");
	TextureManager::getInstance()->loadFromFolder("animations/pause_animation", "Pause");
	TextureManager::getInstance()->loadFromFolder("animations/low_order_success", "Puzzle_End");

}

void PuzzleLevel1Scene::onLoadObjects()
{
	CameraManager::getInstance()->getCurrentCamera()->setPos(Vector2D(0.0f));
	SequenceGameManager::initialize();

	Background* pBG = new Background("Background", "BG", Vector2D(1.0f, 0.9f));
	GameObjectManager::getInstance()->addObject(pBG);

	EmptyObject* pDragonContainer = new EmptyObject("DragonContainer");
	GameObjectManager::getInstance()->addObject(pDragonContainer);
	Sprite* pDragonHead = new Sprite("Dragon_Head", "Dragon_Head", Vector2D(-360.0f, 50.0f), Vector2D(0.6f));
	Sprite* pDragonTail = new Sprite("Dragon_Tail", "Dragon_Tail", Vector2D(450.0f, 50.0f), Vector2D(0.6f));

	//GameObjectManager::getInstance()->addObject(pDragonHead);
	//GameObjectManager::getInstance()->addObject(pDragonTail);
	pDragonContainer->attachChild(pDragonHead);
	pDragonContainer->attachChild(pDragonTail);

	Sprite* pDragonTile1 = new Sprite("Dragon_Tile_1", "Dragon_Tile", Vector2D(-410.0f, 50.0f), Vector2D(0.95f));
	Sprite* pDragonTile2 = new Sprite("Dragon_Tile_2", "Dragon_Tile", Vector2D(-30.0f, 50.0f), Vector2D(0.95f));
	Sprite* pDragonTile3 = new Sprite("Dragon_Tile_3", "Dragon_Tile", Vector2D(350.0f, 50.0f), Vector2D(0.95f));

	Text* pTileText1 = new Text("Tile_Text_1",  "Maragsa.otf", 200,0,false);
	//GameObjectManager::getInstance()->addObject(pTileText1);
	pDragonTile1->attachChild(pTileText1);
	pTileText1->setMessage("1");
	pTileText1->setPos(Vector2D(-380.0f, 30.0f));

	Text* pTileText2 = new Text("Tile_Text_2", "Maragsa.otf", 200, 0, false);
	//GameObjectManager::getInstance()->addObject(pTileText2);
	pDragonTile2->attachChild(pTileText2);
	pTileText2->setMessage("2");
	pTileText2->setPos(Vector2D(0.0f, 30.0f));

	Text* pTileText3 = new Text("Tile_Text_3", "Maragsa.otf", 200, 0, false);
	//GameObjectManager::getInstance()->addObject(pTileText3);
	pDragonTile3->attachChild(pTileText3);
	pTileText3->setMessage("3");
	pTileText3->setPos(Vector2D(380.0f, 30.0f));

	pTileText1->setColor({ 100, 31, 31, 255 });
	pTileText2->setColor({ 100, 31, 31, 255 });
	pTileText3->setColor({ 100, 31, 31, 255 });

	//GameObjectManager::getInstance()->addObject(pDragonTile1);
	//GameObjectManager::getInstance()->addObject(pDragonTile2);
	//GameObjectManager::getInstance()->addObject(pDragonTile3);
	pDragonContainer->attachChild(pDragonTile1);
	pDragonContainer->attachChild(pDragonTile2);
	pDragonContainer->attachChild(pDragonTile3);

	
	Sprite* pBlank1 = new Sprite("Token_Blank_1", "Token_Blank", Vector2D(-280.0f, -375.0f), Vector2D(0.45f));
	Sprite* pBlank2 = new Sprite("Token_Blank_2", "Token_Blank", Vector2D(0.0f, -375.0f), Vector2D(0.45f));
	Sprite* pBlank3 = new Sprite("Token_Blank_3", "Token_Blank", Vector2D(280.0f, -375.0f), Vector2D(0.45f));

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

	Sprite* pCompleteDragon = new Sprite("Complete_Dragon", "Full_Dragon", Vector2D(50, 75), Vector2D(.55));
	GameObjectManager::getInstance()->addObject(pCompleteDragon);
	SequenceGameManager::getInstance()->setCombination(pTokenClaws->getName(), pTokenHorns->getName(), pTokenEyes->getName());//set the win con
	pCompleteDragon->setEnabled(false);
	GUIButton* pPauseButton = new GUIButton("Pause_Button", "Pause_Button");
	pPauseButton->setPos(Vector2D(800.0f, 400.0f));
	pPauseButton->setScale(Vector2D(0.08f));
	GUIToggle* pPauseToggle = new GUIToggle(EventKey::PAUSE_SCREEN);
	pPauseButton->attachComponent(pPauseToggle);

	GUIButton* pStoryButton = new GUIButton("Story_Button", "Story_Button");
	pStoryButton->setPos(Vector2D(600.0f, 400.0f));
	pStoryButton->setScale(Vector2D(0.08f));
	GUIToggle* pStoryToggle = new GUIToggle(EventKey::STORY_SCREEN);
	pStoryButton->attachComponent(pStoryToggle);

	GameObjectManager::getInstance()->addObject(pPauseButton);
	GameObjectManager::getInstance()->addObject(pStoryButton);


	PuzzleHint* pHint = new PuzzleHint("PuzzleHintWindow");
	pHint->setEnabled(false);
	GameObjectManager::getInstance()->addObject(pHint);

	PauseScreen* pPauseScreen = new PauseScreen("PauseScreen");
	GameObjectManager::getInstance()->addObject(pPauseScreen);
	pPauseScreen->setEnabled(true);
	 
	ObjectiveIntro* pObjectiveIntro = new ObjectiveIntro("L1_Obj", "Obejctive:\nForm the Bakunawa!\nRelics to sequence: 3");
	pObjectiveIntro->setIsScreenObject(true);
	pObjectiveIntro->setPos(Vector2D(0,0));
	GameObjectManager::getInstance()->addObject(pObjectiveIntro);

	std::string strMessage = "Once upon a time, in the ancient Philippines, Bathala—a supreme god—created seven moons that lit up the earth.\n\n They floated across the night sky, turning darkness into beauty and silence into wonder. \n\nBut deep beneath the waves was a scary creature.That creature was Bakunawa, a great serpent - dragon of the sea, who ruled\n over the ocean. His body wrapped around the world, and his home was cold and shadowy.\n\nOne night, he rose from the depths and saw the moons for the first time.\n\nHe was amazed.\n\nTheir soft glow lit the world in ways he had never known.They were so bright... so beautiful...\n\nAnd he wanted them—all of them.\n\nAnd so, one night, Bakunawa leapt from the black ocean and flew into the sky.\nHe opened his massive jaws and swallowed one moon whole.\n\nAs the moon disappeared, the Earth trembled.A terrible earthquake shook the night.\nBakunawa returned to the sea, full and satisfied.\nBut soon, the moon inside him melted away, like wax near fire.\n\nFurious, Bakunawa tried again the next night.He devoured a second moon.\nAnd again... it melted.\n\n“There are so many moons, ” he hissed.“I will eat them all!”\n\nNight after night, the serpent struck.And night after night, another moon vanished.\n\nSix moons were gone.\n\n\nOnly one remained.\n\nThe people of the ancient islands were afraid.\n\nEach time a moon vanished, earthquakes shook their homes, and darkness filled the skies.\n\nThey knew Bakunawa would return—and they had to stop him\n\nSo they learned the sea dragon feared loud noises.\n\nSo they gathered their pots, pans, gongs, and drums.And they waited.\n\nOn a moonless night, Bakunawa rose once more from the sea.\n\nWith a thunderous cry, he soared upwards, mouth opened wide, and wrapped his teeth around the seventh and final moon.\n\nThe world began to dim.Shadows spread across the land.It was almost gone.\n\nBut then—BANG! CLANG! BOOM!\n\nThe people ran to the serpent dragon, shouting:\n\n“Return our moon!”\n\nThey banged pots and pans, beat gongs,\n and yelled into the sky.The noise thundered across the land and sea.\n\nStartled and afraid, Bakunawa spat out the moon just before\n swallowing it whole.He roared and went back into the deep and dark ocean.\n\nThe moon rose again, casting light on the land.The people cheered and danced beneath its glow.\n\nLittle did Bakunawa know, Bathala had seen everything.\n\nTo guard the moon forever, Bathala planted tall bamboo trees on its surface.From afar,\n they looked like dark spots—shadows that still remain today.\n\nBakunawa never gave up.Even now, sometimes, he tries to steal the moon again.\n\nAnd whenever he does, the people make noise—banging drums, lighting fireworks, and shouting to the skies\n until the moon is safe once more.\n\nBecause as long as the bamboo trees stand, and the people remember…\n\nBakunawa will never succeed.";

	StoryWindow* pStoryWindow = new StoryWindow("Puzzle1StoryWindow");
	pStoryWindow->setPos(Vector2D(0, 0));
	GameObjectManager::getInstance()->addObject(pStoryWindow);
	pStoryWindow->getTitleText()->setMessage("Bakunawa and the Seven Moons");
	pStoryWindow->getStoryText()->setMessage(strMessage);
	pStoryWindow->setEnabled(false);

	PuzzleEnd* pEnding = new PuzzleEnd("EndScreen", pDragonContainer,pCompleteDragon);
	GameObjectManager::getInstance()->addObject(pEnding);


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
	TextureManager::getInstance()->unload("Close_Button");
	TextureManager::getInstance()->unload("Tablet");
	TextureManager::getInstance()->unload("Intro");
	TextureManager::getInstance()->unload("Pause");
	FontManager::getInstance()->unloadAllFonts();
	EventBroadcaster::getInstance()->unregisterAllListeners();
}
