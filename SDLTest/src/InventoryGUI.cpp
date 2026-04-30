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
#include "DataAssetManager.h"

InventoryGUI::InventoryGUI(std::string strName):AGameObject(strName)
{
	this->pDataAsset = (CollectableGemDataAsset*)DataAssetManager::getInstance()->getDataAsset("CollectableGemDataAsset");
	this->EKey = EventKey::INVENTORY_SCREEN;
	this->bListenerEnabled = true;
	this->bIsScreenObject = true;
	this->pPlayer = NULL;

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
	pTitle->setIsScreenObject(true);
	pGemInfo->attachChild(pTitle);
	pTitle->setMessage("No Gem");
	pTitle->setPos(Vector2D(1050, 250));
	pTitle->setScale(Vector2D(1, 1));
	pTitle->setRot(-14);

	Text* pText = new Text("Text", "JainiPurva-Regular.ttf", 45, 0, false);
	pText->setIsScreenObject(true);
	pGemInfo->attachChild(pText);
	pText->setMessage("Please Select a Gem");
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

	SpriteRenderer* pGRenderer;
	GUIButton* pCyanGem = new GUIButton("Gem_Cyan", "Gem_Cyan", false);
	pCyanGem->setIsScreenObject(true);
	pTransBack->attachChild(pCyanGem);
	this->vecGems.push_back(pCyanGem);
	GUIToggle* pCyanToggle = new GUIToggle(EventKey::INVENTORY_SCREEN);
	pCyanGem->attachComponent(pCyanToggle);
	pCyanGem->setScale(Vector2D(.35f));
	pCyanGem->setPos(Vector2D(1825, 225));
	pGRenderer = (SpriteRenderer*)pCyanGem->findComponentByName("SpriteRenderer");
	pGRenderer->setCropRect(SDL_FRect(.175, .1, .65, .7));

	GUIButton* pGreenGem = new GUIButton("Gem_Green", "Gem_Green", false);
	pGreenGem->setIsScreenObject(true);
	pTransBack->attachChild(pGreenGem);
	this->vecGems.push_back(pGreenGem);
	GUIToggle* pGreenToggle = new GUIToggle(EventKey::INVENTORY_SCREEN);
	pGreenGem->attachComponent(pGreenToggle);
	pGreenGem->setScale(Vector2D(.35f));
	pGreenGem->setPos(Vector2D(1825, 375));
	pGRenderer = (SpriteRenderer*)pGreenGem->findComponentByName("SpriteRenderer");
	pGRenderer->setCropRect(SDL_FRect(.175, .1, .65, .7));

	GUIButton* pOrangeGem = new GUIButton("Gem_Orange", "Gem_Orange", false);
	pOrangeGem->setIsScreenObject(true);
	pTransBack->attachChild(pOrangeGem);
	this->vecGems.push_back(pOrangeGem);
	GUIToggle* pOrangeToggle = new GUIToggle(EventKey::INVENTORY_SCREEN);
	pOrangeGem->attachComponent(pOrangeToggle);
	pOrangeGem->setScale(Vector2D(.35f));
	pOrangeGem->setPos(Vector2D(1825, 525));
	pGRenderer = (SpriteRenderer*)pOrangeGem->findComponentByName("SpriteRenderer");
	pGRenderer->setCropRect(SDL_FRect(.175, .1, .65, .7));


	GUIButton* pPurpleGem = new GUIButton("Gem_Purple", "Gem_Purple", false);
	pPurpleGem->setIsScreenObject(true);
	pTransBack->attachChild(pPurpleGem);
	this->vecGems.push_back(pPurpleGem);
	GUIToggle* pPurpleToggle = new GUIToggle(EventKey::INVENTORY_SCREEN);
	pPurpleGem->attachComponent(pPurpleToggle);
	pPurpleGem->setScale(Vector2D(.35f));
	pPurpleGem->setPos(Vector2D(1825, 675));
	pGRenderer = (SpriteRenderer*)pPurpleGem->findComponentByName("SpriteRenderer");
	pGRenderer->setCropRect(SDL_FRect(.175, .1, .65, .7));

	GUIButton* pRedGem = new GUIButton("Gem_Red", "Gem_Red", false);
	pRedGem->setIsScreenObject(true);
	pTransBack->attachChild(pRedGem);
	this->vecGems.push_back(pRedGem);
	GUIToggle* pRedToggle = new GUIToggle(EventKey::INVENTORY_SCREEN);
	pRedGem->attachComponent(pRedToggle);
	pRedGem->setScale(Vector2D(.35f));
	pRedGem->setPos(Vector2D(1825, 825));
	pGRenderer = (SpriteRenderer*)pRedGem->findComponentByName("SpriteRenderer");
	pGRenderer->setCropRect(SDL_FRect(.175, .1, .65, .7));

	pTransBack->setEnabled(false);
	pAnimatedCharacter->setEnabled(false);
	pGemInfo->setEnabled(false);

	pAnimator->addListener(this);
	EventBroadcaster::getInstance()->registerListener(this);
}
void InventoryGUI::changeGem(std::string strGemName)
{
	Background* pTransBack = (Background*)this->findChildByName("TransparentBG");
	SpriteRenderer* pRenderer = (SpriteRenderer*)this->findChildByName("InfoGem")->findComponentByName("SpriteRenderer");
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
		for (GUIButton* pGem : this->vecGems)
		{
			if (!this->pDataAsset->getGemPickedUp(pGem->getName()))
			{
				pGem->setEnabled(false);
			}
			else
			{
				pGem->setEnabled(true);
			}
		}
	}
	else if (pTransBack->getEnabled() )
	{
		if(bFromToggle)
		{
			pTransBack->setEnabled(false);
			pAnimSprite->setEnabled(false);
			pGem->setEnabled(false);
			pTitle->setMessage("No Gem");
			pText->setMessage("Please Select a Gem");
			this->changeGem("Gem_Colorless_Inventory");
			EventBroadcaster::getInstance()->enableAllListeners();
		}
		else
		{
			std::string senderType = *static_cast<std::string*>(mapParameter["Sender"]);
			if (senderType.find("Gem", 0) != std::string::npos)
			{
				pTitle->setMessage(this->pDataAsset->getGemTitle(senderType));
				pText->setMessage(this->pDataAsset->getGemTextBreak(senderType));
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
