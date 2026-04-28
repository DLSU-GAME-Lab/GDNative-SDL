#include "TurnCounter.h"
#include "GameObjectManager.h"
#include "EndScreen.h"
#include "AudioManager.h"
#include "Settings.h"
TurnCounter::TurnCounter(std::string strName, int nTurnsLeft, Vector2D fVecPos, Vector2D fVecScale):AGameObject(strName)
{
	this->nTurnsLeft = nTurnsLeft;
	this->EKey = EventKey::COUNTER_SUBTRACT;
	this->fVecPos = fVecPos;
	this->fVecScale = fVecScale;
	this->bCounterEnabled = true;
	this->bListenerEnabled = true;
}

TurnCounter::~TurnCounter()
{
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
	return this->bListenerEnabled;
}

void TurnCounter::setListenerEnabled(bool bListenerEnabled)
{
	this->bListenerEnabled = bListenerEnabled;
}

std::string TurnCounter::getListenerOwnerName()
{
	return this->strName;
}

void TurnCounter::initialize()
{
	Text* pTitleText = new Text(this->strName + "Text", "CurseCasual.ttf", 35, 0, false);
	pTitleText->setMessage("Turns Left: ");
	pTitleText->setColor(colorWhite);
	this->attachChild(pTitleText);
	pTitleText->setPos(this->fVecPos + Vector2D(0, 30));

	Text* pCounterText = new Text(this->strName, "CurseCasual.ttf", 35, 0, false);
	pCounterText->setColor(colorWhite);
	pCounterText->setMessage(std::to_string(this->nTurnsLeft));
	this->attachChild(pCounterText);
	pCounterText->setPos(this->fVecPos);
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
		AudioManager::getInstance()->play(new AudioPlayer("Lose_SFX", AudioGroupTag::SFX));
		AudioManager::getInstance()->play(new AudioPlayer("Lose_Sigh", AudioGroupTag::SFX));
	}
	Text* pText = (Text*)this->findChildByName(this->getName());
	pText->modifyText(std::to_string(this->nTurnsLeft));
}