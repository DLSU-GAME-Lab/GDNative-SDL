#pragma once
#include "EventListener.h"
#include "Vector2D.h"
#include "Text.h"
class EndScreen: public AGameObject, EventListener
{
private:
	EventKey EKey;
	AGameObject* pWinBanner;
	AGameObject* pLoseBanner;

public:
	EndScreen(std::string strName);
	~EndScreen();

	// Inherited via AGameObject
	void initialize() override;

	// Inherited via EventListener
	void onEventTrigger(std::unordered_map<std::string, void*> mapParameter) override;
	EventKey getKey() override;
	virtual bool isListenerEnabled() override;
	virtual void setListenerEnabled(bool bListenerEnabled) override;
	virtual std::string getListenerOwnerName() override;
};

