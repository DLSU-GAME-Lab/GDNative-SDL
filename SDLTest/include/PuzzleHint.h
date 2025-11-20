#pragma once
#include "TextureManager.h"
#include "FontManager.h"
#include "EventBroadcaster.h"
#include "AGameObject.h"
#include "EventListener.h"
#include "Text.h"
#include "Sprite.h"
#include "GUIButton.h"
#include "GUIToggle.h"
class PuzzleHint:public AGameObject, EventListener
{
private:
	Text* pText;
	EventKey EKey;
	bool bListenerEnabled;
public:
	PuzzleHint(std::string strName);
	~PuzzleHint();
	void initialize();

	// Inherited via EventListener
	void onEventTrigger(std::unordered_map<std::string, void*> mapParameter) override;
	EventKey getKey() override;

	// Inherited via EventListener
	bool isListenerEnabled() override;
	void setListenerEnabled(bool bListenerEnabled) override;
	std::string getListenerOwnerName() override;
};

