#include "PlatformerPickupGUI.h"
#include "SpriteAnimator.h" 
#include "EventBroadcaster.h" 
#include "Background.h"
#include "TextureManager.h"
#include "GUIToggle.h"
#include "ButtonInput.h"
#include "Text.h"
PlatformerPickupGUI::PlatformerPickupGUI(std::string strName):AGameObject(strName)
{
	this->EKey = EventKey::ITEM_PICKUP;
	this->bIsListenerEnabled = true;
	this->bIsScreenObject = true;
	this->mapickupDialogue = {};
}

PlatformerPickupGUI::~PlatformerPickupGUI()
{
}

void PlatformerPickupGUI::initialize()
{
	Background* pTransBack = new Background("TransparentBG", "Square", Vector2D(5.f));
	this->attachChild(pTransBack);
	SpriteRenderer* pTransBGR = (SpriteRenderer*)pTransBack->findComponentByName("SpriteRenderer");
	pTransBGR->setColor({ 250, 227, 150, 127 });
	ButtonInput* pInput = new ButtonInput(pTransBGR);
	pTransBack->attachComponent(pInput);
	GUIToggle* pToggle = new GUIToggle(EventKey::ITEM_PICKUP);
	pTransBack->attachComponent(pToggle);

	AnimatedSprite* pAnimatedCharacter = new AnimatedSprite("PickupAnim", "Pickup",Vector2D(0),Vector2D(1.f),0,12);
	pAnimatedCharacter->setIsScreenObject(true);
	this->attachChild(pAnimatedCharacter);
	SpriteAnimator* pAnimator = (SpriteAnimator*)pAnimatedCharacter->findComponentByName("SpriteAnimator");
	SpriteRenderer* pSpriteRenderer = (SpriteRenderer*)pAnimatedCharacter->findComponentByName("SpriteRenderer");
	pSpriteRenderer->setPivot(Vector2D(0, 0));
	pAnimator->getCurrentAnimation()->setType(AnimationType::ONCE);
	pAnimator->getCurrentAnimation()->setOnAnimFinished(OnAnimFinished::FUNC);

	Sprite* pGem = new Sprite("Gem", "Gem_Red_Grab", Vector2D(0), Vector2D(0.125f), 0, false);
	pGem->setIsScreenObject(true);
	pTransBack->attachChild(pGem);
	pGem->setPos(Vector2D(1050, 600));

	Text* pTitle = new Text("Title", "JainiPurva-Regular.ttf",45, 0, false);
	pTitle->setMessage("Character");
	pTitle->setIsScreenObject(true);
	pGem->attachChild(pTitle);
	pTitle->setPos(Vector2D(1250, 450));
	pTitle->setScale(Vector2D(1, 1));

	Text* pText = new Text("Text", "JainiPurva-Regular.ttf", 30, 0, false);
	pText->setMessage("The Young Moth is the Main \n Character");
	pText->setIsScreenObject(true);
	pGem->attachChild(pText);
	pText->setPos(Vector2D(1250, 550));
	pText->setScale(Vector2D(1, 1));

	pTransBack->setEnabled(false);
	pAnimatedCharacter->setEnabled(false);
	pAnimator->addListener(this);
	EventBroadcaster::getInstance()->registerListener(this);
}

void PlatformerPickupGUI::changeGem(std::string strGemName)
{
	Background* pTransBack = (Background*)this->findChildByName("TransparentBG");
	SpriteRenderer* pRenderer = (SpriteRenderer*)pTransBack->findChildByName("Gem")->findComponentByName("SpriteRenderer");
	SDL_Texture* pTex = TextureManager::getInstance()->getTexture(strGemName)[0];
	pRenderer->setTexture(pTex);
}

void PlatformerPickupGUI::onEventTrigger(std::unordered_map<std::string, void*> mapParameter)
{
	
		//Play animation
	Background* pTransBack = (Background*)this->findChildByName("TransparentBG");
	AnimatedSprite* pAnimSprite = (AnimatedSprite*)this->findChildByName("PickupAnim");
	Text* pTitle = (Text*)pTransBack->findChildByName("Gem")->findChildByName("Title");
	Text* pText = (Text*)pTransBack->findChildByName("Gem")->findChildByName("Text");
	SpriteAnimator* pAnimator = (SpriteAnimator*)pAnimSprite->findComponentByName("SpriteAnimator");

	bool bFromToggle = false;
	if (mapParameter.find("Sender") != mapParameter.end())
	{
		std::string senderType = *static_cast<std::string*>(mapParameter["Sender"]);
		bFromToggle = (senderType == "TransparentBG");

	}
	if (!pTransBack->getEnabled())
	{
		pAnimSprite->setEnabled(true);
		EventBroadcaster::getInstance()->disableOtherListenerExcept(this);
		pAnimator->getCurrentAnimation()->stop();
		pAnimator->getCurrentAnimation()->play();
		if (mapParameter.find("GemName") != mapParameter.end())
		{
			std::string strGemName = *static_cast<std::string*>(mapParameter["GemName"]);
			pTitle->modifyText(this->mapickupDialogue[strGemName][0]);
			pText->modifyText(this->mapickupDialogue[strGemName][1]);
			this->changeGem(strGemName+"_Inventory");
		}


	}
	else if (pTransBack->getEnabled() && bFromToggle)
	{

		pTransBack->setEnabled(false);
		pAnimSprite->setEnabled(false);
		EventBroadcaster::getInstance()->enableAllListeners();

	}
	
}

EventKey PlatformerPickupGUI::getKey()
{
	return this->EKey;
}

bool PlatformerPickupGUI::isListenerEnabled()
{
	return this->bIsListenerEnabled;
}

void PlatformerPickupGUI::setListenerEnabled(bool bListenerEnabled)
{
	this->bIsListenerEnabled = bListenerEnabled;
}

std::string PlatformerPickupGUI::getListenerOwnerName()
{
	return this->strName;
}

void PlatformerPickupGUI::addPickupDialogue(std::string strKey, std::string strDialogue)
{
	this->mapickupDialogue[strKey].push_back(strDialogue);
}



void PlatformerPickupGUI::onAnimationFinished()
{
	Background* pTransBack = (Background*)this->findChildByName("TransparentBG");
	pTransBack->setEnabled(true);
}
