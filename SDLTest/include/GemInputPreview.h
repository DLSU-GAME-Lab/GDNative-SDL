#pragma once
#include "AGameObject.h"
#include "EventListener.h"
#include "CollectableGemDataAsset.h"

class GemInputPreview : public AGameObject, public EventListener
{
private:
	EventKey EKey;
	bool bListenerEnabled;
	CollectableGemDataAsset* pDataAsset;

public:
	GemInputPreview();

	void setPreviewText(std::string strGemName);
	
	void initialize() override;
	// Inherited via EventListener
	void onEventTrigger(std::unordered_map<std::string, void*> mapParameter) override;
	EventKey getKey() override;
	bool isListenerEnabled() override;
	void setListenerEnabled(bool bListenerEnabled) override;
	std::string getListenerOwnerName() override;
};

