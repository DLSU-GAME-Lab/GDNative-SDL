#pragma once
#include "AGameObject.h"
#include "EventListener.h"
#include "AnimatedSprite.h"
#include "IAnimatorListener.h"
#include "Prop.h"
#include "Player.h"
class InventoryGUI:public AGameObject, EventListener,IAnimatorListener
{
private:
	EventKey EKey;
	bool bListenerEnabled;
	std::unordered_map <std::string, std::vector<std::string>> mapInventoryDialogue;
	Player* pPlayer;
public:
	InventoryGUI(std::string strName);
	~InventoryGUI();

	// Inherited via AGameObject
	void initialize() override;
	void changeGem(std::string strGemName);

	// Inherited via EventListener
	void onEventTrigger(std::unordered_map<std::string, void*> mapParameter) override;
	EventKey getKey() override;
	bool isListenerEnabled() override;
	void setListenerEnabled(bool bListenerEnabled) override;
	void addPickupDialogue(std::string strKey, std::string strDialogue);
	std::string getListenerOwnerName() override;

	// Inherited via IAnimatorListener
	void onAnimationFinished() override;
};

