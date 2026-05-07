#include "StoreGUI.h"
#include "GameObjectManager.h"
#include "Sprite.h"
#include "GUIUtils.h"
#include "EventBroadcaster.h"
#include "GUIButton.h"
#include "GUIToggle.h"
#include "StoreItem.h"
StoreGUI::StoreGUI(std::string strName):AGameObject(strName)
{
	this->bEnabled = true;
	this->EKey = EventKey::SHOP_TOGGLE;
	this->bListenerEnabled = true;
}

StoreGUI::~StoreGUI()
{
}

void StoreGUI::initialize()
{
	Sprite* pStoreContainer = new Sprite("Store_Container", "UI_Container", Vector2D(0, 0), Vector2D(1, 1), 0.0f, false);
	GUIUtils::setGUIMidCenter(pStoreContainer, Vector2D(-20));
	this->attachChild(pStoreContainer);
	SpriteRenderer* pRenderer = (SpriteRenderer*)pStoreContainer->findComponentByName("SpriteRenderer");
	pRenderer->setNineSlice(true, 42);
	pStoreContainer->setPos(Vector2D(0, -50));
	pStoreContainer->setLocalScale(Vector2D(3, 6));

	GUIButton* pCloseButton = new GUIButton("CloseButton", "Close_Button");
	GUIToggle* pShopCloseToggle = new GUIToggle(EventKey::SHOP_TOGGLE);
	pCloseButton->attachComponent(pShopCloseToggle);
	pStoreContainer->attachChild(pCloseButton);
	pCloseButton->setPos(Vector2D(265, 380));
	pCloseButton->setScale(Vector2D(.5, .5));

	Text* pTitleText = new Text("Store Title", "CurseCasual.ttf", 75, 0, false);
	pTitleText->setMessage("Shop");
	pStoreContainer->attachChild(pTitleText);
	pTitleText->setLocalPos(Vector2D(0, 390));
	pTitleText->setScale(Vector2D(1, 1));

	StoreItem* pItem1 = new StoreItem("Coins", 0, Vector2D(0,240));
	pStoreContainer->attachChild(pItem1);
	EventBroadcaster::getInstance()->registerListener(this);

}

void StoreGUI::onEventTrigger(std::unordered_map<std::string, void*> mapParameter)
{
	std::string name = *static_cast<std::string*>(mapParameter["Sender"]);

	if(this->getEnabled() && name == "CloseButton")
	{
		this->setEnabled(false);
		EventBroadcaster::getInstance()->enableAllListeners();

	}
	else
	{
		this->setEnabled(true);
		EventBroadcaster::getInstance()->disableOtherListenerExcept(this);
		EventBroadcaster::getInstance()->enableListener("StoreManager");

	}

}

EventKey StoreGUI::getKey()
{
	return this->EKey;
}

bool StoreGUI::isListenerEnabled()
{
	return this->bListenerEnabled;
}

void StoreGUI::setListenerEnabled(bool bListenerEnabled)
{
	this->bListenerEnabled = bListenerEnabled;
}

std::string StoreGUI::getListenerOwnerName()
{
	return this->strName;
}
