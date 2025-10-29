#include "PuzzleHint.h"

PuzzleHint::PuzzleHint(std::string strName):AGameObject(strName)
{
	this->EKey = EventKey::RIGHT_CLICK;
	this->pText = NULL;
}

PuzzleHint::~PuzzleHint()
{
	TextureManager::getInstance()->unload("HintPage");
}

void PuzzleHint::initialize()
{
	TextureManager::getInstance()->load("sequence_game/Popup_Panel.png", "HintPage");
	Prop* pProp = new Prop("HintPage", "HintPage", Vector2D(0), Vector2D(1), 0);
	this->attachChild(pProp);

	this->pText = new Text("TextHint", "Place Holder", "Maragsa.otf", 90);
	pText->setPos(Vector2D(0, 50));
	pText->setScale(Vector2D(.75, .75));
	this->attachChild(this->pText);

	GUIButton* pButton = new GUIButton("CloseButton", "Close_Button");
	GUIToggle* pToggle = new GUIToggle(EventKey::RIGHT_CLICK);
	pButton->setPos(Vector2D(0, -300));
	pButton->setScale(Vector2D(.15, .15));
	pButton->attachComponent(pToggle);
	this->attachChild(pButton);


	Text* pDeclineText = new Text("Decline_Text", "Close", "JainiPurva-Regular.ttf", 90, 0.f, false);
	pDeclineText->setPos(Vector2D(0, -300));
	pDeclineText->setScale(Vector2D(.75, .75));
	this->attachChild(pDeclineText);

	EventBroadcaster::getInstance()->registerListener(this);
}
void PuzzleHint::onEventTrigger(std::unordered_map<std::string, void*> mapParameter)
{
	bool bFromToggle = false;
	if (mapParameter.find("Sender") != mapParameter.end())
	{
		std::string senderType = *static_cast<std::string*>(mapParameter["Sender"]);
		bFromToggle = (senderType == "CloseButton");
		std::cout << senderType << std::endl;
	}

	if(!this->bEnabled)
	{
		EventBroadcaster::getInstance()->disableOtherListenerExcept(this);
		std::string strName = *static_cast<std::string*>(mapParameter["TokenName"]);
		if (strName == "Token_Horns")
		{
			this->pText->modifyText(" \"Bathala planted tall bamboo trees so that\n Bakunawawould not be able to swallow the\n moon so easily. Theses bamboo trees looked\nlike stains or dark spots on the surface of the\n moon from afar.\"");
			this->setEnabled(true);
		}
		else if (strName == "Token_Claws")
		{
			this->pText->modifyText(" \"Bakunawa sprung from the pitch-black \n ocean and boted towards the sky he opened \n his mouth wide as he could around one of\n the moons and with one powerful swoop,\n swallowed it whole. As he did so a huge wave\n of earthquakes shook the ancient land of\n the Philippines.\"");
			this->setEnabled(true);


		}
		else if (strName == "Token_Eyes")
		{
			this->pText->modifyText(" \"The ancient Filipinos ran to the ocean while\n banging their pots and pans and shouted\n together, \"Return our moon\". Bakunawa was\n so startled by the loud sudden noise that it \n spat out the seventh moon.\"");
			this->setEnabled(true);
		}
	}
	else if(this->bEnabled && bFromToggle)
	{
		this->setEnabled(false);
	}
}

EventKey PuzzleHint::getKey()
{
	return this->EKey;
}
