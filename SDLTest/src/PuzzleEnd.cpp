#include "PuzzleEnd.h"
#include "Background.h"
#include "SpriteRenderer.h"
#include "AnimatedSprite.h"
#include "SpriteAnimator.h"
#include "GUIButton.h"
#include "SceneSwitcher.h"
#include "Sprite.h"
#include "EventBroadcaster.h"
#include "Text.h"

PuzzleEnd::PuzzleEnd(std::string strName, AGameObject* incDragon, AGameObject* compDragon) :AGameObject(strName)
{
	this->EKey = EventKey::END_LEVEL;
	this->bListenerEnabled = true;
	this->bIsScreenObject = true;
	this->incDragon = incDragon;
	this->compDragon = compDragon;
}

PuzzleEnd::~PuzzleEnd()
{
	this->incDragon = NULL;
	this->compDragon = NULL;
}

void PuzzleEnd::initialize()
{
	Background* pTransBack = new Background("TransparentBG", "Square", Vector2D(5.f));

	this->attachChild(pTransBack);
	SpriteRenderer* pTransBGR = (SpriteRenderer*)pTransBack->findComponentByName("SpriteRenderer");
	pTransBGR->setColor({ 255, 255, 255, 0 });

	AnimatedSprite* pAnimSprite = new AnimatedSprite("EndAnim", "Puzzle_End", Vector2D(0, 0.0f), Vector2D(1), 0.0f, 12);
	pAnimSprite->setIsScreenObject(true);
	this->attachChild(pAnimSprite);
	SpriteAnimator* pAnimator = (SpriteAnimator*)pAnimSprite->findComponentByName("SpriteAnimator");
	SpriteRenderer* pSpriteRenderer = (SpriteRenderer*)pAnimSprite->findComponentByName("SpriteRenderer");
	pSpriteRenderer->setPivot(Vector2D(0, 0));
	pAnimator->getCurrentAnimation()->setType(AnimationType::ONCE);
	pAnimator->getCurrentAnimation()->setOnAnimFinished(OnAnimFinished::FUNC);

	GUIButton* pExit = new GUIButton("Button_Exit", "Close_Button");
	pExit->setIsScreenObject(true);
	pExit->setPos(Vector2D(1000, 800));
	pExit->setScale(Vector2D(.025, .025));
	SceneSwitcher* pExitSwitch = new SceneSwitcher(SceneTag::LOBBY_SCENE);
	pExit->attachComponent(pExitSwitch);
	pTransBack->attachChild(pExit);

	Text* pExitText = new Text("Exit Text", "Maragsa.otf", 200);
	pExit->attachChild(pExitText);
	pExitText->setMessage("Return");

	Sprite* pTextHolder = new Sprite("pTextHolder", "Tablet");
	pTextHolder->setIsScreenObject(true);
	pTextHolder->setPos(Vector2D(1000, 300));
	pTextHolder->setScale(Vector2D(.05, .05));
	pTransBack->attachChild(pTextHolder);

	Text* CompleteText = new Text("Level Complete", "Maragsa.otf", 200);
	pTextHolder->attachChild(CompleteText);
	CompleteText->setMessage("Level Complete");

	pTransBack->setEnabled(false);
	pAnimSprite->setEnabled(false);
	pAnimator->addListener(this);
	EventBroadcaster::getInstance()->registerListener(this);

}

void PuzzleEnd::onEventTrigger(std::unordered_map<std::string, void*> mapParameter)
{
	Background* pTransBack = (Background*)this->findChildByName("TransparentBG");
	AnimatedSprite* pAnimSprite = (AnimatedSprite*)this->findChildByName("EndAnim");
	SpriteAnimator* pAnimator = (SpriteAnimator*)pAnimSprite->findComponentByName("SpriteAnimator");
	if (!pTransBack->getEnabled())
	{
		pAnimSprite->setEnabled(true);
		EventBroadcaster::getInstance()->disableOtherListenerExcept(this);
		pAnimator->getCurrentAnimation()->stop();
		pAnimator->getCurrentAnimation()->play();
	}
}

EventKey PuzzleEnd::getKey()
{
	return this->EKey;
}

bool PuzzleEnd::isListenerEnabled()
{
	return this->bListenerEnabled;
}

void PuzzleEnd::setListenerEnabled(bool bListenerEnabled)
{
	this->bListenerEnabled = bListenerEnabled;
}

std::string PuzzleEnd::getListenerOwnerName()
{
	return this->getName();
}

void PuzzleEnd::onAnimationFinished()
{
	this->incDragon->setEnabled(false);
	this->compDragon->setEnabled(true);
	AnimatedSprite* pAnimSprite = (AnimatedSprite*)this->findChildByName("EndAnim");
	pAnimSprite->setEnabled(false);
	Background* pTransBack = (Background*)this->findChildByName("TransparentBG");
	pTransBack->setEnabled(true);
}


