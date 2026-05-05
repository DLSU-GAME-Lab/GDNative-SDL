#include "StoreGUI.h"
#include "GameObjectManager.h"
#include "Sprite.h"
#include "GUIUtils.h"
#include "EventBroadcaster.h"
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
	Sprite* pLowerUIContainer = new Sprite("Lower_UI_Container", "UI_Container", Vector2D(0, 0), Vector2D(10, 2), 0.0f, false);
	GUIUtils::setGUIMidCenter(pLowerUIContainer, Vector2D(-20));
	GameObjectManager::getInstance()->addObject(pLowerUIContainer);

	EventBroadcaster::getInstance()->registerListener(this);

}

void StoreGUI::onEventTrigger(std::unordered_map<std::string, void*> mapParameter)
{
	if(this->bListenerEnabled)
	{
		this->bEnabled = !this->bEnabled;
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
