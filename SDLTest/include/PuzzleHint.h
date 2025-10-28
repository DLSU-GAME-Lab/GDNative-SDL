#pragma once
#include "TextureManager.h"
#include "FontManager.h"
#include "EventBroadcaster.h"
#include "AGameObject.h"
#include "EventListener.h"
#include "Text.h"
#include "Prop.h"
#include "GUIButton.h"
#include "GUIToggle.h"
class PuzzleHint:public AGameObject, EventListener
{
private:
	Text* pText;
	EventKey EKey;
public:
	PuzzleHint(std::string strName);
	~PuzzleHint();
	void initialize();

	// Inherited via EventListener
	void onEventTrigger(std::unordered_map<std::string, void*> mapParameter) override;
	EventKey getKey() override;
};

