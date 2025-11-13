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

	Background* pParentBG = new Background("TransparentBG", "Square", Vector2D( 1.5f));
	pParentBG->setPos(Vector2D(500, 200));
	this->attachChild(pParentBG);
	SpriteRenderer* pParentBGR = (SpriteRenderer*)pParentBG->findComponentByName("SpriteRenderer");
	pParentBGR->setColor({ 255, 255, 255, 120 });
	ButtonInput* pInput = new ButtonInput(pParentBGR);
	pParentBG->attachComponent(pInput);

	Text* pTitle = new Text("TitleText", "JainiPurva-Regular.ttf", 90, 0, false);
	pTitle->setMessage("Story");
	pTitle->setPos(Vector2D(0, 450));
	this->attachChild(pTitle);

	Text* pLineBreak = new Text("LineBreak", "JainiPurva-Regular.ttf", 90, 0, false);
	pLineBreak->setMessage("----------------------------------------");
	pLineBreak->setPos(Vector2D(0, 400));
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
	

}

void StoryWindow::onEventTrigger(std::unordered_map<std::string, void*> mapParameter)
{
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
