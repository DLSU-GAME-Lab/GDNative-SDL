#include "PauseScreen.h"
#include "SpriteAnimator.h"

PauseScreen::PauseScreen(std::string strName): AGameObject(strName)
{
	this->EKey = EventKey::PAUSE_SCREEN;
	this->bListenerEnabled = true;
	this->bIsScreenObject = true;
}
PauseScreen::~PauseScreen()
{
	TextureManager::getInstance()->unload("BrownButton");
}
void PauseScreen::initialize()
{
	Background* pTransBack = new Background("TransparentBG", "Square", Vector2D(5.f));

	this->attachChild(pTransBack);
	SpriteRenderer* pTransBGR = (SpriteRenderer*)pTransBack->findComponentByName("SpriteRenderer");
	pTransBGR->setColor({ 250, 227, 150, 127 });

	Text* pPauseText = new Text("PauseText", "JainiPurva-Regular.ttf", 120, -15);
	pPauseText->setIsScreenObject(true);
	pPauseText->setMessage("Paused");
	pPauseText->setPos(Vector2D(550, 200));
	pPauseText->setColor(SDL_Color(165, 42, 42, 255));
	pTransBack->attachChild(pPauseText);

	TextureManager::getInstance()->load("GUI/button5.png", "BrownButton");
	GUIButton* pResume = new GUIButton("ResumeButton", "BrownButton", false);
	pResume->setIsScreenObject(true);
	GUIToggle* pToggle = new GUIToggle(EventKey::PAUSE_SCREEN);
	pResume->setRot(-17.5);
	pResume->setScale(Vector2D(.15, .15));
	pResume->setPos(Vector2D(500 , 350));
	pResume->attachComponent(pToggle);
	pTransBack->attachChild(pResume);

	Text* pResumeText = new Text("ResumeText", "JainiPurva-Regular.ttf", 45, -15);
	pResumeText->setIsScreenObject(true);
	pResumeText->setMessage("Resume");
	pResumeText->setRot(-17.5);
	pResumeText->setPos(Vector2D(500, 350));
	pResumeText->setColor(SDL_Color(255, 255, 255, 255));
	pResume->attachChild(pResumeText);

	GUIButton* pQuit = new GUIButton("QuitButton", "BrownButton", false);
	pQuit->setIsScreenObject(true);
	SceneSwitcher* pSceneSwitcher = new SceneSwitcher(SceneTag::LOBBY_SCENE);
	pQuit->setRot(-17);
	pQuit->setScale(Vector2D(.15, .15));
	pQuit->setPos(Vector2D(500, 550));
	pQuit->attachComponent(pSceneSwitcher);
	pTransBack->attachChild(pQuit);

	Text* pQuitText = new Text("QuitText", "JainiPurva-Regular.ttf", 45, -15);
	pQuitText->setIsScreenObject(true);
	pQuitText->setMessage("Quit Stage");
	pQuitText->setRot(-17);
	pQuitText->setPos(Vector2D(500, 550));
	pQuitText->setColor(SDL_Color(255, 255, 255, 255));

	pQuit->attachChild(pQuitText);
	AnimatedSprite* pAnimSprite = new AnimatedSprite("PauseAnim", "Pause", Vector2D(250, 0.0f), Vector2D(0.9f), 0.0f, 12);
	pAnimSprite->setIsScreenObject(true);
	this->attachChild(pAnimSprite);
	SpriteAnimator* pAnimator = (SpriteAnimator*)pAnimSprite->findComponentByName("SpriteAnimator");
	SpriteRenderer* pSpriteRenderer = (SpriteRenderer*)pAnimSprite->findComponentByName("SpriteRenderer");
	pSpriteRenderer->setPivot(Vector2D(0, 0));
 	pAnimator->getCurrentAnimation()->setType(AnimationType::ONCE);
	pAnimator->getCurrentAnimation()->setOnAnimFinished(OnAnimFinished::FUNC);

	pTransBack->setEnabled(false);
	pAnimSprite->setEnabled(false);
	pAnimator->addListener(this);
	EventBroadcaster::getInstance()->registerListener(this);

}

void PauseScreen::onEventTrigger(std::unordered_map<std::string, void*> mapParameter)
{
	//Play animation
	Background* pTransBack = (Background*)this->findChildByName("TransparentBG");
	AnimatedSprite* pAnimSprite = (AnimatedSprite*)this->findChildByName("PauseAnim");
	SpriteAnimator* pAnimator = (SpriteAnimator*)pAnimSprite->findComponentByName("SpriteAnimator");

	bool bFromToggle = false;
	if (mapParameter.find("Sender") != mapParameter.end())
	{
		std::string senderType = *static_cast<std::string*>(mapParameter["Sender"]);
		bFromToggle = (senderType == "ResumeButton");

	}
	if (!pTransBack->getEnabled())
	{
		pAnimSprite->setEnabled(true);
		EventBroadcaster::getInstance()->disableOtherListenerExcept(this);
		pAnimator->getCurrentAnimation()->stop();
		pAnimator->getCurrentAnimation()->play();

	
	}
	else if (pTransBack->getEnabled() && bFromToggle)
	{

		pTransBack->setEnabled(false);
		pAnimSprite->setEnabled(false);
		EventBroadcaster::getInstance()->enableAllListeners();

	}

}

EventKey PauseScreen::getKey()
{
	return this->EKey;
}

bool PauseScreen::isListenerEnabled()
{
	return this->bListenerEnabled;
}

void PauseScreen::setListenerEnabled(bool bListenerEnabled)
{
	this->bListenerEnabled = bListenerEnabled;
}

std::string PauseScreen::getListenerOwnerName()
{
	return this->strName;
}

void PauseScreen::onAnimationFinished()
{
	Background* pTransBack = (Background*)this->findChildByName("TransparentBG");
	pTransBack->setEnabled(true);
}
