#pragma once
#include "EventListener.h"
#include "IAnimatorListener.h"
#include "AGameObject.h"
#include "AnimatedSprite.h"

#include "Sprite.h"
class PlatformerPickupGUI:public AGameObject, EventListener,IAnimatorListener
{
private:
	EventKey EKey;
	bool bIsListenerEnabled;
	std::unordered_map <std::string, std::vector<std::string>> mapickupDialogue;
public: 
	PlatformerPickupGUI(std::string strName);
	~PlatformerPickupGUI();
	void initialize() override;
	void changeGem(std::string strGemName);
	// Inherited via EventListener
	void onEventTrigger(std::unordered_map<std::string, void*> mapParameter) override;
	EventKey getKey() override;
	bool isListenerEnabled() override;
	void setListenerEnabled(bool bListenerEnabled) override;
	std::string getListenerOwnerName() override;
	void addPickupDialogue(std::string strKey, std::string strDialogue);
	// Inherited via IAnimatorListener
	void onAnimationFinished() override;
};

