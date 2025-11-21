#include "EndScreen.h"
#include "GameObjectManager.h"
#include "TrackerManager.h"
#include "EventBroadcaster.h"
#include "TurnCounter.h"
#include "GUIButton.h"
#include "SceneSwitcher.h"
#include "Sprite.h"
EndScreen::EndScreen(std::string strName):AGameObject(strName)
{
	this->bEnabled = true;
	this->EKey = EventKey::GAME_END;
	this->bListenerEnabled = true;
	this->pWinBanner = NULL;
	this->pLoseBanner = NULL;
}

EndScreen::~EndScreen()
{

}

void EndScreen::initialize()
{

	this->pWinBanner = new Sprite("VictoryRibbon", "Victory_Ribbon", Vector2D(0, 200), Vector2D(1, 1), 0.f, false);
	this->pWinBanner->setEnabled(false);
	this->attachChild(this->pWinBanner);

	this->pLoseBanner = new Sprite("DefeatRibbon", "Defeat_Ribbon", Vector2D(0, 200), Vector2D(1, 1), 0.f, false);
	this->pLoseBanner->setEnabled(false);
	this->attachChild(this->pLoseBanner);

	GUIButton* pReturn = new GUIButton("ReturnButton", "Green_Button", false);
	pReturn->setPos(Vector2D(0, -100));
	this->attachChild(pReturn);
	SceneSwitcher* pSwitcher = new SceneSwitcher(SceneTag::TITLE_SCENE);
	pReturn->attachComponent(pSwitcher);

	Text* pVictoryText = new Text("VictoryText", "CurseCasual.ttf", 90, 0, false);
	pVictoryText->setMessage("Victory");
	pVictoryText->setColor(SDL_Color(255, 223, 0, 255));
	pWinBanner->attachChild(pVictoryText);
	pVictoryText->setPos(Vector2D(0, 250));

	Text* pDefeatText = new Text("DefeatText", "CurseCasual.ttf", 90, 0, false);
	pDefeatText->setMessage("Defeat");
	pDefeatText->setColor(SDL_Color(255, 223, 0, 255));
	pLoseBanner->attachChild(pDefeatText);
	pDefeatText->setPos(Vector2D(0, 250));

	Text* pReturnText = new Text("ReturnText", "CurseCasual.ttf", 45, 0, false);
	pReturnText->setMessage("Level Select");
	pReturnText->setColor(SDL_Color(255, 255, 255, 255));
	pReturn->attachChild(pReturnText);
	EventBroadcaster::getInstance()->registerListener(this);
	pDefeatText->setPos(Vector2D(0, -100));


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
		this->pLoseBanner->setEnabled(false);
	}
	else
	{
		this->bEnabled = true;
		this->pLoseBanner->setEnabled(true);
		this->pWinBanner->setEnabled(false);

	}
}

EventKey EndScreen::getKey()
{
	return this->EKey;
}

bool EndScreen::isListenerEnabled()
{
	return this->bListenerEnabled;
}

void EndScreen::setListenerEnabled(bool bListenerEnabled)
{
	this->bListenerEnabled = bListenerEnabled;
}

std::string EndScreen::getListenerOwnerName()
{
	return this->strName;
}
