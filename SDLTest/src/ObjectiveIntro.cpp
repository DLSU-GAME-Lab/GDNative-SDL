#include "ObjectiveIntro.h"

ObjectiveIntro::ObjectiveIntro(std::string strName, std::string strMessage):AGameObject(strName)
{
	this->strMessage = strMessage;
	this->EKey = EventKey::OBJECTIVE_SCREEN;
	this->bListenerEnabled = true;
}

ObjectiveIntro::~ObjectiveIntro()
{
}

void ObjectiveIntro::initialize()
{
	Background* pTransBG = new Background("Transparent_BG", "Square", Vector2D(5.0f));
	this->attachChild(pTransBG);
	SpriteRenderer* pTransBGR = (SpriteRenderer*)pTransBG->findComponentByName("SpriteRenderer");
	pTransBGR->setColor({ 0, 0, 0, 127 });

	Prop* pTablet = new Prop("Tablet", "Tablet", Vector2D(0.0f), Vector2D(0.9f));
	pTransBG->attachChild(pTablet);

	Text* pTabletText1 = new Text("Tablet_Text", "JainiPurva-Regular.ttf", 90, 0, false);
	pTabletText1->setMessage(this->strMessage);
	pTabletText1->setPos(Vector2D(-500.0f, 75));
	pTablet->attachChild(pTabletText1);
	DialogueRenderer* pTabletTextR1 = (DialogueRenderer*)pTabletText1->findComponentByName("DialogueRenderer");
	pTabletTextR1->setPivot(Vector2D(0.0f, 0.5f));

	AnimatedSprite* pIntro = new AnimatedSprite("Intro", "Intro", Vector2D(100.0f, 0.0f), Vector2D(0.9f), 0.0f, 12);
	pTransBG->attachChild(pIntro);
	SpriteAnimator* pAnimator = (SpriteAnimator*)pIntro->findComponentByName("SpriteAnimator");
	pAnimator->getCurrentAnimation()->setType(AnimationType::ONCE);
	this->setEnabled(true);

	GUIButton* pClose = new GUIButton("Close", "Close_Button");
	pClose->setPos(Vector2D(0, -450));
	pClose->setScale(Vector2D(.15, .15));
	GUIToggle* pToggle = new GUIToggle(EventKey::OBJECTIVE_SCREEN);
	pClose->attachComponent(pToggle);
	this->attachChild(pClose);

	Text* pCloseText = new Text("Tablet_Text", "JainiPurva-Regular.ttf", 90, 0, false);
	pCloseText->setMessage("CLose");
	pCloseText->setPos(Vector2D(0, -430));
	pClose->attachChild(pCloseText);
	DialogueRenderer* pCloseTextR1 = (DialogueRenderer*)pCloseText->findComponentByName("DialogueRenderer");

;
	
	EventBroadcaster::getInstance()->registerListener(this);
	EventBroadcaster::getInstance()->disableOtherListenerExcept(this);
}

void ObjectiveIntro::onEventTrigger(std::unordered_map<std::string, void*> mapParameter)
{
	if (this->bEnabled)
	{
		this->setEnabled(false);
		std::cout << "Closing" << std::endl;
		EventBroadcaster::getInstance()->enableAllListeners();
	}



}

EventKey ObjectiveIntro::getKey()
{
	return this->EKey;
}

bool ObjectiveIntro::isListenerEnabled()
{
	return this->bListenerEnabled;
}

void ObjectiveIntro::setListenerEnabled(bool bListenerEnabled)
{
	this->bListenerEnabled = bListenerEnabled;
}

std::string ObjectiveIntro::getListenerOwnerName()
{
	return this->strName;
}
