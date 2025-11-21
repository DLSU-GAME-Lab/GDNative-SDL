#pragma once
#include "AComponent.h"
#include "EnumEventKey.h"
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

