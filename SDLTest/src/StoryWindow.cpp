#include "StoryWindow.h"

StoryWindow::StoryWindow(std::string strName):AGameObject(strName)
{
	this->EKey = EventKey::STORY_SCREEN;
	this->bListenerEnabled = true;
	this->bIsScreenObject = true;
	this->pMessage = NULL;
	this->pTitle = NULL;
}

StoryWindow::~StoryWindow()
{
	this->pMessage = NULL;
}

void StoryWindow::initialize()
{
	Background* pTransBack = new Background("TransparentBG", "Square", Vector2D(5.f));
	this->attachChild(pTransBack);
	SpriteRenderer* pTransBGR = (SpriteRenderer*)pTransBack->findComponentByName("SpriteRenderer");
	pTransBGR->setColor({ 250, 227, 150, 127 });

	Background* pParentBG = new Background("ParentBG", "Square", Vector2D( 5,1.25f));
	pParentBG->setPos(Vector2D(0, 200));
	this->attachChild(pParentBG);
	SpriteRenderer* pParentBGR = (SpriteRenderer*)pParentBG->findComponentByName("SpriteRenderer");
	pParentBGR->setColor({ 255, 255, 255, 0 });
	ButtonInput* pInput = new ButtonInput(pParentBGR);
	pParentBG->attachComponent(pInput);

	this->pTitle = new Text("TitleText", "JainiPurva-Regular.ttf", 90, 0, false);
	this->attachChild(this->pTitle);
	this->pTitle->setIsScreenObject(true);
	this->pTitle->setPos(Vector2D(1000, 100));
	this->pTitle->setScale(Vector2D(1, 1));
	this->pTitle->setPos(Vector2D(1000, 100));

	Text* pLineBreak = new Text("LineBreak", "JainiPurva-Regular.ttf", 90, 0, false);
	this->attachChild(pLineBreak);
	pLineBreak->setIsScreenObject(true);
	pLineBreak->setMessage("----------------------------------------");
	pLineBreak->setPos(Vector2D(1000, 150));
	pLineBreak->setScale(Vector2D(1, 1));
	
	this->pMessage = new Text("Message", "JainiPurva-Regular.ttf", 45, 0, false);
	pParentBG->attachChild(this->pMessage);
	this->pMessage->setIsScreenObject(true);
	this->pMessage->setPos(Vector2D(0, 0));
	this->pMessage->setScale(Vector2D(1, 1));

	TextRenderer* pRenderer = ((TextRenderer*)this->pMessage->findComponentByName("TextRenderer"));
	pRenderer->setCropEnabled(true);
	pRenderer->setPivot(Vector2D(0, 0));
	pRenderer->setViewHeight(pParentBGR->getRect().h);

	Scroller* pScroller = new Scroller("StoryScroller", this->pMessage);
	pParentBG->attachComponent(pScroller);
	pScroller->setVert(true);

	GUIButton* pButton = new GUIButton("CloseButton", "Close_Button");
	GUIToggle* pToggle = new GUIToggle(EventKey::STORY_SCREEN);
	pButton->setIsScreenObject(true);
	pButton->attachComponent(pToggle);
	this->attachChild(pButton);
	pButton->setScale(Vector2D(.15, .15));

	Text* pDeclineText = new Text("Decline_Text", "JainiPurva-Regular.ttf", 90, 0.f, false);
	pButton->attachChild(pDeclineText);
	pDeclineText->setIsScreenObject(true);
	pDeclineText->setMessage("Close");
	pButton->setPos(Vector2D(1000, 950));
	pDeclineText->setScale(Vector2D(.75, .75));


	
	EventBroadcaster::getInstance()->registerListener(this);

}

Text* StoryWindow::getStoryText()
{
	return this->pMessage;
}

Text* StoryWindow::getTitleText()
{
	return this->pTitle;
}

void StoryWindow::onEventTrigger(std::unordered_map<std::string, void*> mapParameter)
{
	bool bFromToggle = false;
	Text* pText = (Text*)this->findChildByName("ParentBG")->findChildByName("Message");
	if (mapParameter.find("Sender") != mapParameter.end())
	{
		std::string senderType = *static_cast<std::string*>(mapParameter["Sender"]);
		bFromToggle = (senderType == "CloseButton");
		std::cout << senderType << std::endl;
	}

	if (!this->bEnabled)
	{
		EventBroadcaster::getInstance()->disableOtherListenerExcept(this);
		this->setEnabled(true);
	}
	else if (this->bEnabled && bFromToggle)
	{
		this->setEnabled(false);
		pText->setPos(Vector2D(0,0));
		EventBroadcaster::getInstance()->enableAllListeners();
	}
}

EventKey StoryWindow::getKey()
{
	return this->EKey;
}

bool StoryWindow::isListenerEnabled()
{
	return this->bListenerEnabled;
}

void StoryWindow::setListenerEnabled(bool bListenerEnabled)
{
	this->bListenerEnabled = bListenerEnabled;
}

std::string StoryWindow::getListenerOwnerName()
{
	return this->getName();
}
