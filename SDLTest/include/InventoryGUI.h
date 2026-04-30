#pragma once
#include "AGameObject.h"
#include "EventListener.h"
#include "AnimatedSprite.h"
#include "IAnimatorListener.h"
#include "Sprite.h"
#include "Player.h"
#include "GUIButton.h"
#include "CollectableGemDataAsset.h"

class InventoryGUI:public AGameObject, EventListener,IAnimatorListener
{
private:
	CollectableGemDataAsset* pDataAsset;
	EventKey EKey;
	bool bListenerEnabled;
	Player* pPlayer;
	std::vector<GUIButton*> vecGems;
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
	void adjustFlag(std::string key, bool bFlag);
	std::unordered_map <std::string, bool> getFlags();
	// Inherited via IAnimatorListener
	void onAnimationFinished() override;
};

