#include "PauseScreen.h"

PauseScreen::PauseScreen(std::string strName): AGameObject(strName)
{
	this->EKey = EventKey::PAUSE_SCREEN;
	this->bListenerEnabled = true;
}
PauseScreen::~PauseScreen()
{
	TextureManager::getInstance()->unload("BrownButton");
}
void PauseScreen::initialize()
{
	Background* pTransBack = new Background("TransparentBG", "Square", Vector2D(5.f));
	this->attachChild(pTransBack);
	SpriteRenderer* pTransBGR = (SpriteRenderer*)pTransBack->findComponentByName("SpriteRenderer");
	pTransBGR->setColor({ 250, 227, 150, 127 });

	//Background* pLeftShape = new Background("Brown", "Square", Vector2D(3.f));
	//this->attachChild(pLeftShape);
	//SpriteRenderer* pLeftShapeR = (SpriteRenderer*)pLeftShape->findComponentByName("SpriteRenderer");
	//pLeftShape->setRot(75.f);
	//pLeftShape->setPos(Vector2D( -1000, -600));
	//pLeftShapeR->setColor({ 92, 64, 51, 255 });

	Text* pPauseText = new Text("PauseText", "JainiPurva-Regular.ttf", 120, -15);
	pPauseText->setMessage("Paused");
	pPauseText->setPos(Vector2D(-600, 350));
	pPauseText->setColor(SDL_Color(165, 42, 42, 255));
	this->attachChild(pPauseText);

	TextureManager::getInstance()->load("button5.png", "BrownButton");
	GUIButton* pResume = new GUIButton("ResumeButton", "BrownButton", false);
	GUIToggle* pToggle = new GUIToggle(EventKey::PAUSE_SCREEN);
	pResume->setRot(-17);
	pResume->setScale(Vector2D(.15, .15));
	pResume->setPos(Vector2D(-540, 150));
	pResume->attachComponent(pToggle);
	this->attachChild(pResume);

	Text* pResumeText = new Text("ResumeText", "JainiPurva-Regular.ttf", 45, -15);
	pResumeText->setMessage("Resume");
	pResumeText->setRot(-17);
	pResumeText->setPos(Vector2D(-540, 150));
	pResumeText->setColor(SDL_Color(255, 255, 255, 255));
	this->attachChild(pResumeText);

	GUIButton* pQuit = new GUIButton("QuitButton", "BrownButton", false);
	SceneSwitcher* pSceneSwitcher = new SceneSwitcher(SceneTag::RIGHT_ROOM_SCENE);
	pQuit->setRot(-17);
	pQuit->setScale(Vector2D(.15, .15));
	pQuit->setPos(Vector2D(-500, 0));
	pQuit->attachComponent(pSceneSwitcher);
	this->attachChild(pQuit);

	Text* pQuitText = new Text("QuitText", "JainiPurva-Regular.ttf", 45, -15);
	pQuitText->setMessage("Quit Stage");
	pQuitText->setRot(-17);
	pQuitText->setPos(Vector2D(-500, 0));
	pQuitText->setColor(SDL_Color(255, 255, 255, 255));

	this->attachChild(pQuitText);

	EventBroadcaster::getInstance()->registerListener(this);

}

void PauseScreen::onEventTrigger(std::unordered_map<std::string, void*> mapParameter)
{
	//Play animation
	bool bFromToggle = false;
	if (mapParameter.find("Sender") != mapParameter.end())
	{
		std::string senderType = *static_cast<std::string*>(mapParameter["Sender"]);
		bFromToggle = (senderType == "ResumeButton");

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

EventKey PauseScreen::getKey()
{
	return this->EKey;
}

bool PauseScreen::isListenerEnabled()
{
	return this->bListenerEnabled;
}

void PauseScreen::setListenerEnabled(bool bListenerEnabled)
{
	this->bListenerEnabled = bListenerEnabled;
}

std::string PauseScreen::getListenerOwnerName()
{
	return this->strName;
}
