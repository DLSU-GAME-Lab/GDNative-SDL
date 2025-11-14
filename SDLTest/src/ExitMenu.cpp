#include "ExitMenu.h"

ExitMenu::ExitMenu(std::string strName):AGameObject(strName)
{
    this->EKey = EventKey::RETURN_SCREEN;
	this->bListenerEnabled = true;
	this->bIsScreenObject = true;
}

ExitMenu::~ExitMenu()
{
}

void ExitMenu::initialize()
{
	Background* pExitBG = new Background("Exit_Menu_BG", "Return_Dialogue_Holder", Vector2D(.5, .5));
	pExitBG->setPos(Vector2D(575, 250));

	this->attachChild(pExitBG);
	Text* pExitText = new Text("Exit_Text", "JainiPurva-Regular.ttf", 90, 0.f, false);
	pExitText->setIsScreenObject(true);
	pExitText->setMessage("Go Back to Title \n Screen ?");
	pExitText->setPos(Vector2D(950, 450));
	pExitText->setScale(Vector2D(1, 1));
	pExitBG->attachChild(pExitText);


	GUIButton* pDecline = new GUIButton("Decline", "Button_Choices");
	pDecline->setIsScreenObject(true);

	pDecline->setScale(Vector2D(.15, .15));
	pExitBG->attachChild(pDecline);

	GUIToggle* pToggle = new GUIToggle(EventKey::RETURN_SCREEN);

	pDecline->attachComponent(pToggle);

	Text* pDeclineText = new Text("Decline_Text", "JainiPurva-Regular.ttf", 90, 0.f, false);
	pDeclineText->setIsScreenObject(true);

	pDeclineText->setMessage("No");
	pDeclineText->setScale(Vector2D(.75, .75));
	pDecline->attachChild(pDeclineText);
	pDecline->setPos(Vector2D(750, 800));


	GUIButton* pAccept = new GUIButton("Accept", "Button_Choices");
	pAccept->setIsScreenObject(true);

	pAccept->setScale(Vector2D(.15, .15));
	pExitBG->attachChild(pAccept);

	SceneSwitcher* pTitleSwitch = new SceneSwitcher(SceneTag::TITLE_SCENE);
	pAccept->attachComponent(pTitleSwitch);

	Text* pAcceptText = new Text("Accept_Text", "JainiPurva-Regular.ttf", 90, 0.f, false);
	pAcceptText->setIsScreenObject(true);

	pAcceptText->setMessage("Yes");
	pAcceptText->setScale(Vector2D(.75, .75));
	pAccept->attachChild(pAcceptText);
	pAccept->setPos(Vector2D(1250, 800));



	pExitBG->setEnabled(false);
	EventBroadcaster::getInstance()->registerListener(this);
}

void ExitMenu::onEventTrigger(std::unordered_map<std::string, void*> mapParameter)
{
	bool bFromToggle = false;
	if (mapParameter.find("Sender") != mapParameter.end())
	{
		std::string senderType = *static_cast<std::string*>(mapParameter["Sender"]);
		bFromToggle = (senderType == "Decline");
		std::cout << senderType << std::endl;
	}
	if (!this->bEnabled)
	{
		this->setEnabled(true);
		EventBroadcaster::getInstance()->disableOtherListenerExcept(this);
	}
	else if (this->bEnabled && bFromToggle)
	{
		this->setEnabled(false);
		EventBroadcaster::getInstance()->enableAllListeners();
	}
}

EventKey ExitMenu::getKey()
{
    return this->EKey;
}

bool ExitMenu::isListenerEnabled()
{
	return this->bListenerEnabled;
}

void ExitMenu::setListenerEnabled(bool bListenerEnabled)
{
	this->bListenerEnabled = bListenerEnabled;
}

std::string ExitMenu::getListenerOwnerName()
{
	return this->strName;
}
