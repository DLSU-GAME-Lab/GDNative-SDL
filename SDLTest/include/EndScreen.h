#pragma once
#include "EventListener.h"
#include "Vector2D.h"
#include "Prop.h"
#include "Text.h"
class EndScreen: public AGameObject, EventListener
{
private:
	EventKey EKey;
	Prop* pWinBanner;
	Prop* pLoseBanner;
public:
	EndScreen(std::string strName);
	~EndScreen();

	// Inherited via AGameObject
	void initialize() override;

	// Inherited via EventListener
	void onEventTrigger(std::unordered_map<std::string, void*> mapParameter) override;
	EventKey getKey() override;
};

