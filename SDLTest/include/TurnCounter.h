#pragma once
#include "AGameObject.h"
#include "EventListener.h"
#include "Text.h"
class TurnCounter:public AGameObject, EventListener
{
private:
	int nTurnsLeft;
	Vector2D fVecPos;
	Vector2D fVecScale;
	EventKey EKey;
public:
	TurnCounter(std::string strName, int nTurnsLeft, Vector2D fVecPos, Vector2D fVecScale);
	~TurnCounter();
private:
	void updateCounter(int nSubtractValue);

public:
	void initialize();

	// Inherited via EventListener
	void onEventTrigger(std::unordered_map<std::string, void*> mapParameter) override;
	EventKey getKey() override;

};

