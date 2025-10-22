#include "EndScreen.h"
#include "GameObjectManager.h"
#include "TrackerManager.h"
#include "EventBroadcaster.h"
#include "TurnCounter.h"
#include "GUIButton.h"
#include "SceneSwitcher.h"
EndScreen::EndScreen(std::string strName):AGameObject(strName)
{
	this->bEnabled = true;
	this->EKey = EventKey::GAME_END;
}

EndScreen::~EndScreen()
{
	EventBroadcaster::getInstance()->unregisterListener(this);

}

void EndScreen::initialize()
{

	this->pWinBanner = new Prop("VictoryRibbon", "Victory_Ribbon", Vector2D(0, 200), Vector2D(1, 1), 0.f, false);
	this->pWinBanner->setFollowParent(false);
	this->pWinBanner->setEnabled(false);
	this->attachChild(this->pWinBanner);

	this->pLoseBanner = new Prop("DefeatRibbon", "Defeat_Ribbon", Vector2D(0, 200), Vector2D(1, 1), 0.f, false);
	this->pLoseBanner->setFollowParent(false);
	this->pLoseBanner->setEnabled(false);
	this->attachChild(this->pLoseBanner);

	GUIButton* pReturn = new GUIButton("ReturnButton", "Green_Button", false);
	pReturn->setPos(Vector2D(0, -100));
	this->attachChild(pReturn);
	SceneSwitcher* pSwitcher = new SceneSwitcher(SceneTag::TITLE_SCENE);
	pReturn->attachComponent(pSwitcher);

	Text* pVictoryText = new Text("VictoryText", "Victory", Vector2D(0, 250), Vector2D(2, 2), 0, false);
	pVictoryText->setFont("Curse45");
	pVictoryText->setColor(SDL_Color(255, 223, 0, 255));
	pWinBanner->attachChild(pVictoryText);

	Text* pDefeatText = new Text("DefeatText", "Defeat", Vector2D(0, 250), Vector2D(2, 2), 0, false);
	pDefeatText->setFont("Curse45");
	pDefeatText->setColor(SDL_Color(255, 223, 0, 255));
	pLoseBanner->attachChild(pDefeatText);

	Text* pReturnText = new Text("ReturnText", "Level Select", Vector2D(0, -100), Vector2D(1, 1), 0, false);
	pReturnText->setFont("Curse45");
	pReturnText->setColor(SDL_Color(255, 255, 255, 255));
	pReturn->attachChild(pReturnText);
	EventBroadcaster::getInstance()->registerListener(this);

}

void EndScreen::onEventTrigger(std::unordered_map<std::string, void*> mapParameter)
{
	bool bVictory = *static_cast<bool*>(mapParameter["isVictory"]);
	TurnCounter* pCounter = (TurnCounter*)GameObjectManager::getInstance()->findObjectByName("TurnCounter");
	TrackerManager::getInstance()->disableTracker();
	pCounter->disableCounter();
	if (bVictory)
	{
		this->bEnabled = true;
		this->pWinBanner->setEnabled(true);
	}
	else
	{
		this->bEnabled = true;
		this->pLoseBanner->setEnabled(true);
	}
}

EventKey EndScreen::getKey()
{
	return this->EKey;
}
