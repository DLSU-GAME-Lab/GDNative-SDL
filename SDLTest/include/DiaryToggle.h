#pragma once
#include "AComponent.h"
#include "EventBroadcaster.h"
class DiaryToggle : public AComponent
{
private:
	EventKey EKey;
public:
	DiaryToggle(EventKey EKey);
	~DiaryToggle();
public:
	void perform() override;
};

