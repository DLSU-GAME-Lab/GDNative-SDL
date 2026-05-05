#pragma once
#include "AComponent.h"
#include "EnumEventKey.h"
#include "EventBroadcaster.h"
class GUIToggle : public AComponent
{
private:
	EventKey EKey;
public:
	GUIToggle(EventKey EKey);
	~GUIToggle();

	void perform() override;
};

