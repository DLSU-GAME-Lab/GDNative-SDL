#pragma once
#include "AGameObject.h"
#include "GameObjectManager.h"
#include "EventListener.h"
#include "EventBroadcaster.h"
#include "Background.h"
#include "Text.h"
#include "GUIButton.h"
#include "GUIToggle.h"
#include "SceneSwitcher.h"
#include "TextureManager.h"
#include "CameraManager.h"
#include "AnimatedSprite.h"
#include "IAnimatorListener.h"
class PauseScreen:public AGameObject, EventListener, IAnimatorListener
{
private:
	EventKey EKey;
	bool bListenerEnabled;
public:
	PauseScreen(std::string strName);
	~PauseScreen();
	void initialize() override;

	// Inherited via EventListener
	void onEventTrigger(std::unordered_map<std::string, void*> mapParameter) override;
	EventKey getKey() override;

	// Inherited via EventListener
	bool isListenerEnabled() override;
	void setListenerEnabled(bool bListenerEnabled) override;
	std::string getListenerOwnerName() override;

	// Inherited via IAnimatorListener
	void onAnimationFinished() override;
};

