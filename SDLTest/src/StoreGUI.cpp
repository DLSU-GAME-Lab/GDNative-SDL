#include "StoreGUI.h"

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
}

void StoreGUI::onEventTrigger(std::unordered_map<std::string, void*> mapParameter)
{
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
