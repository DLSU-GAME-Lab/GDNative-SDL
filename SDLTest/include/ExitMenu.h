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
public:
	ExitMenu(std::string strName);
	~ExitMenu();
	void initialize();

	// Inherited via EventListener
	void onEventTrigger(std::unordered_map<std::string, void*> mapParameter) override;
	EventKey getKey() override;
};

