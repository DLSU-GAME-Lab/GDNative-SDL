#include "GemInputScreen.h"
#include "Background.h"
#include "GUIButton.h"
#include "Sprite.h"
#include "Text.h"
#include "GameObjectManager.h"
#include "EventBroadcaster.h"
#include "GemInputToken.h"
#include "GemInputManager.h"
#include "GUIToggle.h"
#include "ButtonInput.h"
#include "GemInputPreview.h"

GemInputScreen::GemInputScreen() : AGameObject("GemInputScreen")
{
	this->bIsScreenObject = true;
	this->EKey = EventKey::GEM_INPUT_SCREEN;
	this->bListenerEnabled = true;
}

void GemInputScreen::initialize()
{
	Background* pBG = new Background("GemInputBG", "typing_screen", Vector2D(0.96f, 0.9f));
	this->attachChild(pBG);

	Sprite* pTopSprite = new Sprite("TopSprite", "typing_top", Vector2D(0, 0), Vector2D(1.f, 0.9f));
	this->attachChild(pTopSprite);
	GUIUtils::setGUITopCenter(pTopSprite, Vector2D(0, 20));

	Sprite* pTabletSprite = new Sprite("TabletSprite", "typing_tablet", Vector2D(0, 0), Vector2D(0.95f, 0.85f));
	this->attachChild(pTabletSprite);
	GUIUtils::setGUIMidCenter(pTabletSprite, Vector2D(0, 70));

	Sprite* pCirclesBG = new Sprite("CirclesBG", "Square", Vector2D(0, 0), Vector2D(0.9f, 0.65f));
	pCirclesBG->setIsScreenObject(true);
	this->attachChild(pCirclesBG);
	pCirclesBG->setPos(Vector2D(500, 525));
	pCirclesBG->setColor({ 90, 45, 20, 200 });

	std::string gemNames[] = {
		"Gem_Red",
		"Gem_Orange",
		"Gem_Purple",
		"Gem_Green",
		"Gem_Cyan"
	};
	GemInputManager::getInstance()->addWinCon(4);
	GemInputManager::getInstance()->addWinCon(3);
	GemInputManager::getInstance()->addWinCon(1);
	GemInputManager::getInstance()->addWinCon(2);
	GemInputManager::getInstance()->addWinCon(0);
	std::string textMessage[] = {
		"Somebody...",
		"Wanted...",
		"But...",
		"So...",
		"Then..."
	};

	Vector2D circlePositions[] = {
		Vector2D(350, 430),
		Vector2D(650, 430),
		Vector2D(500, 525),
		Vector2D(350, 620),
		Vector2D(650, 620)
	};

	for (int i = 0; i < 5; i++)
	{
		std::string squareName = "GemInput_Square" + std::to_string(i + 1);
		Sprite* pSquareSprite = new Sprite(squareName, "Square", Vector2D(0, 0), Vector2D(1.4f, 0.15f));
		pSquareSprite->setIsScreenObject(true);
		this->attachChild(pSquareSprite);
		GUIUtils::setGUIMidRight(pSquareSprite, Vector2D(-380, -150 + (i * 100)));
		pSquareSprite->setColor({ 90, 45, 20, 255 });
		GemInputManager::getInstance()->addTextBox(pSquareSprite);

		std::string textName = "GemInput_SquareText" + std::to_string(i + 1);
		Text* pSquareText = new Text(textName, "JainiPurva-Regular.ttf", 40, 0.f, false);
		pSquareText->setIsScreenObject(true);
		this->attachChild(pSquareText);
		pSquareText->setPivot(Vector2D(0.0f, 0.5f));
		pSquareText->setMessage(textMessage[i]);
		pSquareText->setColor({ 255, 255, 255, 127 });
		pSquareText->setPos(Vector2D(850, 385 + (i * 100)));
		GemInputManager::getInstance()->addText(pSquareText);

		std::string buttonName = "GemInput_Undo" + std::to_string(i + 1);
		GUIButton* pUndo = new GUIButton(buttonName, "Return");
		pUndo->setIsScreenObject(true);
		this->attachChild(pUndo);
		GUIUtils::setGUIMidRight(pUndo, Vector2D(-280, -150 + (i * 100)));
		pUndo->setScale(Vector2D(0.035f));
		ButtonInput* pUndoInput = (ButtonInput*)pUndo->findComponentByName("ButtonInput");
		GemInputManager::getInstance()->addUndoButton(pUndoInput);

		std::string circleName = "CircleSprite" + std::to_string(i + 1);
		Sprite* pCircleSprite = new Sprite(circleName, "Circle", circlePositions[i], Vector2D(0.4f));
		pCircleSprite->setIsScreenObject(true);
		this->attachChild(pCircleSprite);
		pCircleSprite->setColor({ 60, 0, 8, 255 });
	}

	for (int i = 0; i < 5; i++)
	{
		GemInputToken* pToken = new GemInputToken(gemNames[i], circlePositions[i] + Vector2D(0, 10), Vector2D(.3f));
		this->attachChild(pToken);
		GemInputManager::getInstance()->addToken(pToken);
	}

	Text* pTitleText = new Text("TitleText", "JainiPurva-Regular.ttf", 80, 0.f, false);
	this->attachChild(pTitleText);
	pTitleText->setMessage("Map SWBST");
	pTitleText->setPos(Vector2D(960, 115));

	GUIButton* pReturn = new GUIButton("GemInput_Return", "Return");
	this->attachChild(pReturn);
	GUIUtils::setGUITopLeft(pReturn, Vector2D(50));
	pReturn->setScale(Vector2D(0.07f));
	GUIToggle* pReturnToggle = new GUIToggle(EventKey::GEM_INPUT_SCREEN);
	pReturn->attachComponent(pReturnToggle);

	GUIButton* pRecall = new GUIButton("GemInput_Recall", "Tablet");
	this->attachChild(pRecall);
	GUIUtils::setGUITopRight(pRecall, Vector2D(-120, 130));
	pRecall->setScale(Vector2D(0.12f));

	GUIButton* pScore = new GUIButton("GemInput_Score", "Tablet");
	this->attachChild(pScore);
	GUIUtils::setGUIBotRight(pScore, Vector2D(-300, -10));
	pScore->setScale(Vector2D(0.13f));

	GUIButton* pSubmit = new GUIButton("GemInput_Submit", "tablet_bottom");
	this->attachChild(pSubmit);
	GUIUtils::setGUIBotCenter(pSubmit, Vector2D(0, -4));
	pSubmit->setScale(Vector2D(0.5f, 0.75f));
	ButtonInput* pSubmitInput = (ButtonInput*)pSubmit->findComponentByName("ButtonInput");
	GemInputManager::getInstance()->addSubmitButton(pSubmitInput);

	GemInputPreview* pPreview = new GemInputPreview();
	this->attachChild(pPreview);
	if (!pPreview->isListenerEnabled())
	{
		std::cout << "enabled" << std::endl;
	}


	EventBroadcaster::getInstance()->registerListener(this);
	this->setEnabled(false);
}

void GemInputScreen::onEnable()
{
	GemInputManager::getInstance()->checkCollection();
}

void GemInputScreen::onEventTrigger(std::unordered_map<std::string, void*> mapParameter)
{
	bool bFromToggle = false;

	if (mapParameter.find("Sender") != mapParameter.end())
	{
		std::string senderType = *static_cast<std::string*>(mapParameter["Sender"]);
		if (senderType == "GemInput_Return" || senderType == "GemInputManager")
		{
			bFromToggle = true;
		}

	}
	if (!this->getEnabled())
	{
		EventBroadcaster::getInstance()->disableOtherListenerExcept(this);
		EventBroadcaster::getInstance()->enableListener("GemInputPreview");
		this->setEnabled(true);

	}
	else
	{
		if (bFromToggle)
		{
			GemInputManager::getInstance()->resetAllText();
			this->setEnabled(false);
			EventBroadcaster::getInstance()->enableAllListeners();
		}
	}
}

EventKey GemInputScreen::getKey()
{
	return this->EKey;
}

bool GemInputScreen::isListenerEnabled()
{
	return this->bListenerEnabled;
}

void GemInputScreen::setListenerEnabled(bool bListenerEnabled)
{
	this->bListenerEnabled = bListenerEnabled;
}

std::string GemInputScreen::getListenerOwnerName()
{
	return this->strName;
}
