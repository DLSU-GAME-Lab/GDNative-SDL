#include "ObjectiveIntro.h"

ObjectiveIntro::ObjectiveIntro(std::string strName, std::string strMessage):AGameObject(strName)
{
	this->strMessage = strMessage;
	this->EKey = EventKey::OBJECTIVE_SCREEN;
	this->bListenerEnabled = true;
	this->bIsScreenObject = true;
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
	pTablet->setIsScreenObject(true);
	pTransBG->attachChild(pTablet);
	pTablet->setPos(Vector2D(1000, 450));
	pTablet->setScale(Vector2D(1, 1));
	Text* pTabletText1 = new Text("Tablet_Text", "JainiPurva-Regular.ttf", 90, 0, false);
	pTabletText1->setIsScreenObject(true);

	pTabletText1->setMessage(this->strMessage);
	pTablet->attachChild(pTabletText1);
	pTabletText1->setScale(Vector2D(1, 1));
	pTabletText1->setPos(Vector2D(600, 450));
	DialogueRenderer* pTabletTextR1 = (DialogueRenderer*)pTabletText1->findComponentByName("DialogueRenderer");
	pTabletTextR1->setPivot(Vector2D(0.0f, 0.5f));

	AnimatedSprite* pIntro = new AnimatedSprite("Intro", "Intro", Vector2D(0, 0.0f), Vector2D(1), 0.0f, 12);
	pIntro->setIsScreenObject(true);
	pTransBG->attachChild(pIntro);
	pIntro->setPos(Vector2D(200, 0.0f));
	pIntro->setScale(Vector2D(Vector2D(1)));

	SpriteRenderer* pIntroRenderer = (SpriteRenderer*)pIntro->findComponentByName("SpriteRenderer");
	pIntroRenderer->setPivot(Vector2D(0, 0));

	SpriteAnimator* pAnimator = (SpriteAnimator*)pIntro->findComponentByName("SpriteAnimator");
	pAnimator->getCurrentAnimation()->setType(AnimationType::ONCE);

	GUIButton* pClose = new GUIButton("Close", "Close_Button");
	pClose->setIsScreenObject(true);

	GUIToggle* pToggle = new GUIToggle(EventKey::OBJECTIVE_SCREEN);
	pClose->attachComponent(pToggle);
	this->attachChild(pClose);
	pClose->setScale(Vector2D(.15, .15));

	Text* pCloseText = new Text("Tablet_Text", "JainiPurva-Regular.ttf", 90, 0, false);
	pCloseText->setIsScreenObject(true);

	pCloseText->setMessage("CLose");
	pClose->attachChild(pCloseText);
	pCloseText->setScale(Vector2D(1, 1));
	pCloseText->setPos(Vector2D(0, -15));

	pClose->setPos(Vector2D(1000, 975));

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
