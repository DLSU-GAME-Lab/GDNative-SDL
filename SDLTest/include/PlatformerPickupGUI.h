#pragma once
#include "EventListener.h"
#include "IAnimatorListener.h"
#include "AGameObject.h"
#include "AnimatedSprite.h"
#include "Sprite.h"
#include "CollectableGemDataAsset.h"

class PlatformerPickupGUI:public AGameObject, EventListener,IAnimatorListener
{
private:
	CollectableGemDataAsset* pDataAsset;
	EventKey EKey;
	bool bIsListenerEnabled;
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

