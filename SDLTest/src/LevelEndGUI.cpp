#include "LevelEndGUI.h"
#include "Background.h"
#include "AnimatedSprite.h"
#include "SpriteRenderer.h"
#include "SpriteAnimator.h"
#include "EventBroadcaster.h"
#include "SceneSwitcher.h"
#include "GUIButton.h"
#include "Sprite.h"
#include "Text.h"
#include "Settings.h"
LevelEndGUI::LevelEndGUI(std::string strName):AGameObject(strName)
{
	this->EKey = EventKey::END_LEVEL;
	this->bListenerEnabled = true;
	this->bIsScreenObject = true;
}

LevelEndGUI::~LevelEndGUI()
{

}

void LevelEndGUI::initialize()
{
	Background* pTransBack = new Background("TransparentBG", "Square", Vector2D(5.f));

	this->attachChild(pTransBack);
	SpriteRenderer* pTransBGR = (SpriteRenderer*)pTransBack->findComponentByName("SpriteRenderer");
	pTransBGR->setColor({ 255, 255, 255, 0 });

	AnimatedSprite* pAnimSprite = new AnimatedSprite("EndAnim", "End_Level", Vector2D(0, 0.0f), Vector2D(0.9f), 0.0f, 12);
	pAnimSprite->setIsScreenObject(true);
	this->attachChild(pAnimSprite);
	SpriteAnimator* pAnimator = (SpriteAnimator*)pAnimSprite->findComponentByName("SpriteAnimator");
	SpriteRenderer* pSpriteRenderer = (SpriteRenderer*)pAnimSprite->findComponentByName("SpriteRenderer");
	pSpriteRenderer->setPivot(Vector2D(0, 0));
	pAnimator->getCurrentAnimation()->setType(AnimationType::ONCE);
	pAnimator->getCurrentAnimation()->setOnAnimFinished(OnAnimFinished::FUNC);

	GUIButton* pExit = new GUIButton("Button_Exit", "Exit");
	pExit->setIsScreenObject(true);
	pExit->setPos(Vector2D(1500, 800));
	pExit->setScale(Vector2D(.025, .025));
	SceneSwitcher* pExitSwitch = new SceneSwitcher(SceneTag::LOBBY_SCENE);
	pExit->attachComponent(pExitSwitch);
	pTransBack->attachChild(pExit);

	Text* pExitText = new Text("ExitText", "JainiPurva-Regular.ttf", 40, 0.f, false);
	pExit->attachChild(pExitText);
	pExitText->setMessage("Return to Hub");
	pExitText->setScale(Vector2D(1, 1));

	Sprite* pTextHolder = new Sprite("pTextHolder", "Tablet");
	pTextHolder->setIsScreenObject(true);
	pTextHolder->setPos(Vector2D(1500, 450));
	pTextHolder->setScale(Vector2D(.075, .075));
	pTransBack->attachChild(pTextHolder);

	Text* pClearText = new Text("ClearText", "JainiPurva-Regular.ttf", 120, 0.f, false);
	pTextHolder->attachChild(pClearText);
	pClearText->setMessage("Cleared");
	pClearText->setColor(colorWhite);
	pClearText->setScale(Vector2D(1, 1));


	pTransBack->setEnabled(false);
	pAnimSprite->setEnabled(false);
	pAnimator->addListener(this);
	EventBroadcaster::getInstance()->registerListener(this);
}

void LevelEndGUI::onEventTrigger(std::unordered_map<std::string, void*> mapParameter)
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

EventKey LevelEndGUI::getKey()
{
	return this->EKey;
}

bool LevelEndGUI::isListenerEnabled()
{
	return this->bListenerEnabled;
}

void LevelEndGUI::setListenerEnabled(bool bListenerEnabled)
{
	this->bListenerEnabled = bListenerEnabled;
}

std::string LevelEndGUI::getListenerOwnerName()
{
	return this->strName;
}

void LevelEndGUI::onAnimationFinished()
{
	Background* pTransBack = (Background*)this->findChildByName("TransparentBG");
	pTransBack->setEnabled(true);
}
