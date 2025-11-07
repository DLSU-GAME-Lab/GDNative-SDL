#pragma once
#include "AGameObject.h"
#include "Background.h"
#include "GameObjectManager.h"
#include "SpriteRenderer.h"
#include "Prop.h"
#include "Text.h"
#include "AnimatedSprite.h"
#include "SpriteAnimator.h"
#include "EventListener.h"
#include "GUIButton.h"
#include "GUIToggle.h"
class ObjectiveIntro:public AGameObject, EventListener
{
private:
	std::string strMessage;
	EventKey EKey;
	bool bListenerEnabled;
public:
	ObjectiveIntro(std::string strName, std::string strMessage);
	~ObjectiveIntro();
	void initialize() override;


	// Inherited via EventListener
	void onEventTrigger(std::unordered_map<std::string, void*> mapParameter) override;

	EventKey getKey() override;

	bool isListenerEnabled() override;

	void setListenerEnabled(bool bListenerEnabled) override;

	std::string getListenerOwnerName() override;

};

