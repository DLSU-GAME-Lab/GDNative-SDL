#include "TurnCounter.h"
#include "GameObjectManager.h"
#include "EndScreen.h"
TurnCounter::TurnCounter(std::string strName, int nTurnsLeft, Vector2D fVecPos, Vector2D fVecScale):AGameObject(strName)
{
	this->nTurnsLeft = nTurnsLeft;
	this->EKey = EventKey::COUNTER_SUBTRACT;
	this->fVecPos = fVecPos;
	this->fVecScale = fVecScale;
	this->bCounterEnabled = true;
}

TurnCounter::~TurnCounter()
{
	EventBroadcaster::getInstance()->unregisterListener(this);
}

void TurnCounter::onEventTrigger(std::unordered_map<std::string, void*> mapParameter)
{
	if(this->bCounterEnabled)
	{
		this->updateCounter(1);
	}
}

EventKey TurnCounter::getKey()
{
	return EventKey::COUNTER_SUBTRACT;
}

bool TurnCounter::isListenerEnabled()
{
	return false;
}

void TurnCounter::setListenerEnabled(bool bListenerEnabled)
{}

std::string TurnCounter::getListenerOwnerName()
{
	return std::string();
}

void TurnCounter::initialize()
{
	Text* pTitleText = new Text(this->strName + "Text", "Turns Left: ", this->fVecPos + Vector2D(0,30), this->fVecScale, 0, false);
	pTitleText->setFont("Curse45");
	this->attachChild(pTitleText);
	Text* pCounterText = new Text(this->strName, std::to_string(this->nTurnsLeft),this->fVecPos, this->fVecScale, 0, false);
	pCounterText->setFont("Curse45");
	this->attachChild(pCounterText);
	EventBroadcaster::getInstance()->registerListener(this);
}
void TurnCounter::disableCounter()
{
	this->bCounterEnabled = false;
}
void TurnCounter::updateCounter(int nSubtractValue)
{
	int nValueHolder = this->nTurnsLeft;
	this->nTurnsLeft = nValueHolder - nSubtractValue;
	if (this->nTurnsLeft <= 0)
	{
		this->nTurnsLeft = 0;
		std::unordered_map<std::string, void*> victoryParams;

		bool bVictory = false;
		victoryParams["isVictory"] = static_cast<void*>(&bVictory);
		EventBroadcaster::getInstance()->broadcast(EventKey::GAME_END, victoryParams);
	}
	Text* pText = (Text*)this->findChildByName(this->getName());
	pText->modifyText(std::to_string(this->nTurnsLeft));
}