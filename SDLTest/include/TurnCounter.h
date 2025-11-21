#pragma once
#include "AGameObject.h"
#include "EventListener.h"
#include "EventBroadcaster.h"
#include "Text.h"
class TurnCounter:public AGameObject, EventListener
{
private:
	int nTurnsLeft;
	Vector2D fVecPos;
	Vector2D fVecScale;
	EventKey EKey;
	bool bCounterEnabled;
public:
	TurnCounter(std::string strName, int nTurnsLeft, Vector2D fVecPos, Vector2D fVecScale);
	~TurnCounter();
private:
	void updateCounter(int nSubtractValue);

public:
	void initialize();
	void disableCounter();
	// Inherited via EventListener
	void onEventTrigger(std::unordered_map<std::string, void*> mapParameter) override;
	EventKey getKey() override;
	virtual bool isListenerEnabled() override;
	virtual void setListenerEnabled(bool bListenerEnabled) override;
	virtual std::string getListenerOwnerName() override;
};

