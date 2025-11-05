#pragma once
#include "DialogueBox.h"
#include "AComponent.h"
class DialogueToggle:public AComponent
{
private:
	int nCounter;
	EventKey EKey;
public:
	DialogueToggle(EventKey EKey);
	~DialogueToggle();
	// Inherited via AComponent
	void perform() override;
};

