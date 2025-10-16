#include "TurnCounter.h"

TurnCounter::TurnCounter(std::string strName, int nTurnsLeft, Vector2D fVecPos, Vector2D fVecScale):AGameObject(strName)
{
	this->nTurnsLeft = nTurnsLeft;
	this->EKey = EventKey::COUNTER_SUBTRACT;
	this->fVecPos = fVecPos;
	this->fVecScale = fVecScale;
}

TurnCounter::~TurnCounter()
{
}

void TurnCounter::onEventTrigger(std::unordered_map<std::string, void*> mapParameter)
{
	this->updateCounter(1);
}

EventKey TurnCounter::getKey()
{
	return this->EKey;
}

void TurnCounter::initialize()
{
	Text* pTitleText = new Text(this->strName + "Text", "Turns Left: ", this->fVecPos + Vector2D(0,30), this->fVecScale, 0, false);
	pTitleText->setFont("Maragasa45");
	this->attachChild(pTitleText);
	Text* pCounterText = new Text(this->strName, std::to_string(this->nTurnsLeft),this->fVecPos, this->fVecScale, 0, false);
	pCounterText->setFont("Maragasa45");
	this->attachChild(pCounterText);
}
void TurnCounter::updateCounter(int nSubtractValue)
{
	int nValueHolder = this->nTurnsLeft;
	this->nTurnsLeft = nValueHolder - nSubtractValue;
	if (this->nTurnsLeft < 0)
	{
		this->nTurnsLeft = 0;
	}
	Text* pText = (Text*)this->findChildByName(this->getName());
	pText->modifyText(std::to_string(this->nTurnsLeft));
}