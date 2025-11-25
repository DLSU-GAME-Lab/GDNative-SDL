#pragma once
#include "EventListener.h"
#include "AGameObject.h" 
#include "Background.h"
#include "GameObjectManager.h"
#include "Text.h"
#include "GUIButton.h"
#include "GUIToggle.h"
#include "SceneSwitcher.h"
class ExitMenu:public AGameObject, EventListener
{
private:
	EventKey EKey;
	bool bListenerEnabled;
public:
	ExitMenu(std::string strName);
	~ExitMenu();
	void initialize() override;

	// Inherited via EventListener
	void onEventTrigger(std::unordered_map<std::string, void*> mapParameter) override;
	EventKey getKey() override;

	// Inherited via EventListener
	bool isListenerEnabled() override;
	void setListenerEnabled(bool bListenerEnabled) override;
	std::string getListenerOwnerName() override;
};

