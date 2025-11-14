#include "StoryWindow.h"

StoryWindow::StoryWindow(std::string strName):AGameObject(strName)
{
	this->EKey = EventKey::STORY_SCREEN;
	this->bListenerEnabled = true;
	this->bIsScreenObject = true;
}

StoryWindow::~StoryWindow()
{
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

	Text* pTitle = new Text("TitleText", "JainiPurva-Regular.ttf", 90, 0, false);
	pTitle->setIsScreenObject(true);
	pTitle->setMessage("Story");
	pTitle->setPos(Vector2D(1000, 100));
	this->attachChild(pTitle);

	Text* pLineBreak = new Text("LineBreak", "JainiPurva-Regular.ttf", 90, 0, false);
	pLineBreak->setIsScreenObject(true);
	pLineBreak->setMessage("----------------------------------------");
	pLineBreak->setPos(Vector2D(1000, 150));
	this->attachChild(pLineBreak);

	std::string strMessage = "The FitnessGram Pacer Test is a multistage aerobic capacity test that progressively gets more difficult as it continues.\n The 20 meter pacer test will begin in 30 seconds.Line up at the start.The running speed starts slowly but gets faster\n each minute after you hear this signal bodeboop.A sing lap should be completed every time you hear this sound.ding\n Remember to run in a straight line and run as long as possible.The second time you fail to complete a lap before the sound,\nyour test is over.The test will begin on the word start.On your mark.Get ready! Start.ding?\n";

	Text* pMessage = new Text("Message", "JainiPurva-Regular.ttf", 45, 0, false);
	pMessage->setIsScreenObject(true);
	pMessage->setMessage(strMessage + "\n" + strMessage + "\n" + strMessage + "\n"+ strMessage);
	pMessage->setPos(Vector2D(0, 0));
	pParentBG->attachChild(pMessage);

	DialogueRenderer* pRenderer = ((DialogueRenderer*)pMessage->findComponentByName("DialogueRenderer"));
	pRenderer->setCropEnabled(true);
	pRenderer->setPivot(Vector2D(0, 0));
	pRenderer->setViewHeight(pParentBGR->getRect().h);

	Scroller* pScroller = new Scroller("StoryScroller", pMessage);
	pParentBG->attachComponent(pScroller);

	GUIButton* pButton = new GUIButton("CloseButton", "Close_Button");
	GUIToggle* pToggle = new GUIToggle(EventKey::STORY_SCREEN);
	pButton->setIsScreenObject(true);
	pButton->setScale(Vector2D(.15, .15));
	pButton->attachComponent(pToggle);
	this->attachChild(pButton);


	Text* pDeclineText = new Text("Decline_Text", "JainiPurva-Regular.ttf", 90, 0.f, false);
	pDeclineText->setIsScreenObject(true);
	pDeclineText->setMessage("Close");
	pDeclineText->setScale(Vector2D(.75, .75));
	pButton->attachChild(pDeclineText);
	pButton->setPos(Vector2D(1000, 950));


	
	EventBroadcaster::getInstance()->registerListener(this);

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
