#pragma once
#include "AGameObject.h"
#include "Prop.h"
#include "EventListener.h"
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

