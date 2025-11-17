#include "PlatformerPickupGUI.h"
#include "SpriteAnimator.h" 
#include "EventBroadcaster.h" 
#include "Background.h"
#include "TextureManager.h"
PlatformerPickupGUI::PlatformerPickupGUI(std::string strName):AGameObject(strName)
{
	this->EKey = EventKey::ITEM_PICKUP;
	this->bIsListenerEnabled = true;
	this->bIsScreenObject = true;
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

	AnimatedSprite* pAnimatedCharacter = new AnimatedSprite("PickupAnim", "Pickup",Vector2D(0),Vector2D(1.f),0,12);
	pAnimatedCharacter->setIsScreenObject(true);
	this->attachChild(pAnimatedCharacter);
	SpriteAnimator* pAnimator = (SpriteAnimator*)pAnimatedCharacter->findComponentByName("SpriteAnimator");
	SpriteRenderer* pSpriteRenderer = (SpriteRenderer*)pAnimatedCharacter->findComponentByName("SpriteRenderer");
	pSpriteRenderer->setPivot(Vector2D(0, 0));
	pAnimator->getCurrentAnimation()->setType(AnimationType::ONCE);
	pAnimator->getCurrentAnimation()->setOnAnimFinished(OnAnimFinished::FUNC);



	Prop* pGem = new Prop("Gem", "Gem_Red", Vector2D(0), Vector2D(0.5f), 0, false);
	pGem->setIsScreenObject(true);
	pTransBack->attachChild(pGem);
	pGem->setPos(Vector2D(1250, 600));

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
		if (mapParameter.find("GemName") != mapParameter.end())
		{
			std::string strGemName = *static_cast<std::string*>(mapParameter["GemName"]);
			this->changeGem(strGemName);
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

void PlatformerPickupGUI::onAnimationFinished()
{
	Background* pTransBack = (Background*)this->findChildByName("TransparentBG");
	pTransBack->setEnabled(true);
}
