#pragma once
#include "DialogueScreen.h"
#include "AComponent.h"
class DialogueToggle:public AComponent
{
private:
	int nCounter;
	EventKey EKey;
	unsigned int nDialogueKey;
public:
	DialogueToggle(unsigned int nDialogueKey);
	~DialogueToggle();
	// Inherited via AComponent
	void perform() override;
};

