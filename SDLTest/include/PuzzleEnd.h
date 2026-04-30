#pragma once
#include "AGameObject.h"
#include "EventListener.h"
#include "IAnimatorListener.h"
class PuzzleEnd :public AGameObject, EventListener, IAnimatorListener
{
private:
	EventKey EKey;
	bool bListenerEnabled;
	AGameObject* incDragon;
	AGameObject* compDragon;
public:
	PuzzleEnd(std::string strName, AGameObject* incDragon, AGameObject* compDragon);
	~PuzzleEnd();

	// Inherited via AGameObject
	void initialize() override;

	// Inherited via EventListener
	void onEventTrigger(std::unordered_map<std::string, void*> mapParameter) override;
	EventKey getKey() override;
	bool isListenerEnabled() override;
	void setListenerEnabled(bool bListenerEnabled) override;
	std::string getListenerOwnerName() override;

	// Inherited via IAnimatorListener
	void onAnimationFinished() override;

	
};

