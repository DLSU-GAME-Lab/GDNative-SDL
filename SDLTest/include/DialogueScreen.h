#pragma once
#include "AGameObject.h"
#include "EventListener.h"
#include "Text.h"
#include "Sprite.h"
#include "GUIButton.h"
#include <unordered_map>

class DialogueScreen : public AGameObject,EventListener
{
private:
	typedef std::string String;
	struct DialogueOption
	{
		String nextKey;
		String text;
	};
	struct DialogueBranch
	{
		String sprite;
		std::vector<DialogueOption> options;
	};

	typedef std::unordered_map<String, DialogueBranch> DialogueBranchMap;

private:
	Sprite* pDialogueSprite;
	EventKey EKey;
	bool bListenerEnabled;
	DialogueBranchMap mapDialogueBranch;
	String strStartBranchKey;
	String strBranchKey;

	std::vector<GUIButton*> vecButton;
	std::vector<Text*> vecText;

public:
	DialogueScreen(std::string strName);
	~DialogueScreen();
public:
	void initialize() override;
	// Inherited via EventListener
	void onEventTrigger(std::unordered_map<std::string, void*> mapParameter) override;
	EventKey getKey() override;
	bool isListenerEnabled() override;
	void setListenerEnabled(bool bListenerEnabled) override;
	std::string getListenerOwnerName() override;

private:
	void setupButtons();
	void setupDialogueBranches();
	void setDialogue();
	
};

