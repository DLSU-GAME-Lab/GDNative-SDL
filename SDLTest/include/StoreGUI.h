#pragma once
#include "EventListener.h"
#include "Vector2D.h"
#include "Text.h"
class StoreGUI :public AGameObject, EventListener
{
private:
	EventKey EKey;
	bool bListenerEnabled;
public:
	StoreGUI(std::string strName);
	~StoreGUI();

	// Inherited via AGameObject
	void initialize() override;

	// Inherited via EventListener
	void onEventTrigger(std::unordered_map<std::string, void*> mapParameter) override;
	EventKey getKey() override;
	bool isListenerEnabled() override;
	void setListenerEnabled(bool bListenerEnabled) override;
	std::string getListenerOwnerName() override;
};

