#pragma once
#include "AGameObject.h"
#include "GUIUtils.h"
#include "EventListener.h"
class GemInputScreen : public AGameObject, EventListener
{
private:
	EventKey EKey;
	bool bListenerEnabled;
public:
	GemInputScreen();

	void initialize() override;
	void onEnable() override;

	// Inherited via EventListener
	void onEventTrigger(std::unordered_map<std::string, void*> mapParameter) override;
	EventKey getKey() override;
	bool isListenerEnabled() override;
	void setListenerEnabled(bool bListenerEnabled) override;
	std::string getListenerOwnerName() override;
};

