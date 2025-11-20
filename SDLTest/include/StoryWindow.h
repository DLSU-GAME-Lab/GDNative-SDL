#pragma once
#include "AGameObject.h"
#include "Sprite.h"
#include "EventListener.h"
#include "Background.h"
#include "Scroller.h"
#include "Text.h"
#include "EventBroadcaster.h"
#include "GUIButton.h"
#include "GUIToggle.h"
class StoryWindow: public AGameObject, EventListener
{
private:
	EventKey EKey;
	bool bListenerEnabled;
public:
	StoryWindow(std::string strName);
	~StoryWindow();

	// Inherited via AGameObject
	void initialize() override;

	// Inherited via EventListener
	void onEventTrigger(std::unordered_map<std::string, void*> mapParameter) override;
	EventKey getKey() override;
	bool isListenerEnabled() override;
	void setListenerEnabled(bool bListenerEnabled) override;
	std::string getListenerOwnerName() override;
};

