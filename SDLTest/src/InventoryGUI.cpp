#include "InventoryGUI.h"
#include "Background.h"
#include "SpriteRenderer.h"
#include "SpriteAnimator.h"
#include "EventBroadcaster.h"
#include "TextureManager.h"
#include "GameObjectManager.h"
#include "GUIButton.h"
#include "Text.h"
#include "GUIToggle.h"
InventoryGUI::InventoryGUI(std::string strName):AGameObject(strName)
{
	this->EKey = EventKey::INVENTORY_SCREEN;
	this->bListenerEnabled = true;
	this->bIsScreenObject = true;
	this->pPlayer = NULL;
	this->mapInventoryDialogue = {};

}

InventoryGUI::~InventoryGUI()
{
}

void InventoryGUI::initialize()
{
	Background* pTransBack = new Background("TransparentBG", "Square", Vector2D(5.f));
	this->attachChild(pTransBack);
	SpriteRenderer* pTransBGR = (SpriteRenderer*)pTransBack->findComponentByName("SpriteRenderer");
	pTransBGR->setColor({ 250, 227, 150, 127 });

	AnimatedSprite* pAnimatedCharacter = new AnimatedSprite("InventoryAnim", "Inventory", Vector2D(0), Vector2D(1.f), 0, 8);
	pAnimatedCharacter->setIsScreenObject(true);
	this->attachChild(pAnimatedCharacter);
	SpriteAnimator* pAnimator = (SpriteAnimator*)pAnimatedCharacter->findComponentByName("SpriteAnimator");
	SpriteRenderer* pSpriteRenderer = (SpriteRenderer*)pAnimatedCharacter->findComponentByName("SpriteRenderer");
	pSpriteRenderer->setPivot(Vector2D(0, 0));
	pAnimator->getCurrentAnimation()->setType(AnimationType::ONCE);
	pAnimator->getCurrentAnimation()->setOnAnimFinished(OnAnimFinished::FUNC);

	Sprite* pGemInfo = new Sprite("InfoGem", "Gem_Colorless_Inventory", Vector2D(0), Vector2D(.9f), 0, false);
	pGemInfo->setIsScreenObject(true);
	this->attachChild(pGemInfo);
	pGemInfo->setPos(Vector2D(1000, 550));

	Text* pTitle = new Text("Title", "JainiPurva-Regular.ttf", 90, 0, false);
	pTitle->setMessage("No Gem");
	pTitle->setIsScreenObject(true);
	pGemInfo->attachChild(pTitle);
	pTitle->setPos(Vector2D(1050, 250));
	pTitle->setScale(Vector2D(1, 1));
	pTitle->setRot(-14);

	Text* pText = new Text("Text", "JainiPurva-Regular.ttf", 45, 0, false);
	pText->setMessage("Please Select a Gem");
	pText->setIsScreenObject(true);
	pGemInfo->attachChild(pText);
	pText->setPos(Vector2D(1000, 450));
	pText->setScale(Vector2D(1, 1));
	pText->setRot(-14);

	Sprite* pInventoryBG = new Sprite("InventoryBG", "Tablet", Vector2D(0), Vector2D(1.f), 0, false);
	pInventoryBG->setIsScreenObject(true);
	pTransBack->attachChild(pInventoryBG);
	pInventoryBG->setScale(Vector2D(.4, 1.25));
	pInventoryBG->setPos(Vector2D(2000,550));


	GUIButton* pReturn = new GUIButton("ReturnButton", "Return", false);
	pReturn->setIsScreenObject(true);
	pTransBack->attachChild(pReturn);
	GUIToggle* pToggle = new GUIToggle(EventKey::INVENTORY_SCREEN);
	pReturn->attachComponent(pToggle);
	pReturn->setScale(Vector2D(.075f));
	pReturn->setPos(Vector2D(100, 100));

	pTransBack->setEnabled(false);
	pAnimatedCharacter->setEnabled(false);
	pGemInfo->setEnabled(false);

	pAnimator->addListener(this);
	EventBroadcaster::getInstance()->registerListener(this);
}
void InventoryGUI::changeGem(std::string strGemName)
{
	Background* pTransBack = (Background*)this->findChildByName("TransparentBG");
	SpriteRenderer* pRenderer = (SpriteRenderer*)pTransBack->findChildByName("Gem")->findComponentByName("SpriteRenderer");
	SDL_Texture* pTex = TextureManager::getInstance()->getTexture(strGemName)[0];
	pRenderer->setTexture(pTex);
}
void InventoryGUI::onEventTrigger(std::unordered_map<std::string, void*> mapParameter)
{
	Background* pTransBack = (Background*)this->findChildByName("TransparentBG");
	AnimatedSprite* pAnimSprite = (AnimatedSprite*)this->findChildByName("InventoryAnim");
	Sprite* pGem = (Sprite*)this->findChildByName("InfoGem");
	Text* pTitle = (Text*)pGem->findChildByName("Title");
	Text* pText = (Text*)pGem->findChildByName("Text");
	SpriteAnimator* pAnimator = (SpriteAnimator*)pAnimSprite->findComponentByName("SpriteAnimator");

	bool bFromToggle = false;
	if (mapParameter.find("Sender") != mapParameter.end())
	{
		std::string senderType = *static_cast<std::string*>(mapParameter["Sender"]);
		bFromToggle = (senderType == "ReturnButton");

	}
	if (!pTransBack->getEnabled())
	{
		pAnimSprite->setEnabled(true);
		EventBroadcaster::getInstance()->disableOtherListenerExcept(this);
		pAnimator->getCurrentAnimation()->stop();
		pAnimator->getCurrentAnimation()->play();
	}
	else if (pTransBack->getEnabled() )
	{
		if(bFromToggle)
		{
			pTransBack->setEnabled(false);
			pAnimSprite->setEnabled(false);
			pGem->setEnabled(false);

			EventBroadcaster::getInstance()->enableAllListeners();
		}
		else
		{
			std::string senderType = *static_cast<std::string*>(mapParameter["Sender"]);
			if (senderType.find("Gem", 0) != std::string::npos)
			{
				pTitle->modifyText(this->mapInventoryDialogue[senderType][0]);
				pText->modifyText(this->mapInventoryDialogue[senderType][1]);
				this->changeGem(senderType + "_Inventory");
			}

		}
	}
}

EventKey InventoryGUI::getKey()
{
	return this->EKey;
}

bool InventoryGUI::isListenerEnabled()
{
	return this->bListenerEnabled;
}

void InventoryGUI::setListenerEnabled(bool bListenerEnabled)
{
	this->bListenerEnabled = bListenerEnabled;
}

void InventoryGUI::addPickupDialogue(std::string strKey, std::string strDialogue)
{
	this->mapInventoryDialogue[strKey].push_back(strDialogue);
}

std::string InventoryGUI::getListenerOwnerName()
{
	return this->strName;
}

void InventoryGUI::onAnimationFinished()
{
	Background* pTransBack = (Background*)this->findChildByName("TransparentBG");
	Sprite* pGem = (Sprite*)this->findChildByName("InfoGem");
	pTransBack->setEnabled(true);
	pGem->setEnabled(true);
}
