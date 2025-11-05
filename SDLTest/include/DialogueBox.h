#pragma once
#include "AGameObject.h"
#include "Text.h"
#include "Prop.h"
#include "EventListener.h"
#include "Background.h"
#include "SpriteRenderer.h"
#include "ButtonInput.h"
#include "EventBroadcaster.h"
#include "DialogueToggle.h"
class DialogueBox : public AGameObject,EventListener
{
private:
	Prop* pDialogueBox;
	Text* pDialogueText;
	EventKey EKey;
	bool bListenerEnabled;
	std::vector<std::string> vecDialogue;
	int nCounter;
public:
	DialogueBox(std::string strName);
	~DialogueBox();
public:
	void initialize() override;
	// Inherited via EventListener
	void onEventTrigger(std::unordered_map<std::string, void*> mapParameter) override;
	EventKey getKey() override;
	bool isListenerEnabled() override;
	void setListenerEnabled(bool bListenerEnabled) override;
	std::string getListenerOwnerName() override;
	
};

