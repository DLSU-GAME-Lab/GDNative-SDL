#include "DialogueBox.h"

DialogueBox::DialogueBox(std::string strName):AGameObject(strName)
{
    this->pDialogueText = NULL;
    this->pDialogueBox = NULL;
    this->bListenerEnabled = true;
    this->EKey = EventKey::DIALOGUE_SCREEN;
    this->nCounter = 0;
    this->vecDialogue = {};
}

DialogueBox::~DialogueBox()
{
}

void DialogueBox::initialize()
{
    Background* pTransBack = new Background("TransparentBG", "Square", Vector2D(5.f));
    this->attachChild(pTransBack);
    SpriteRenderer* pTransBGR = (SpriteRenderer*)pTransBack->findComponentByName("SpriteRenderer");
    pTransBGR->setColor({ 0, 0, 0, 127 });
    ButtonInput* pBackgroundInput = new ButtonInput(pTransBGR);
    pTransBack->attachComponent(pBackgroundInput);


    this->pDialogueBox = new Prop("DialogueBox", "Square", Vector2D(0, 0), Vector2D(1, 1), 0, false);
    this->pDialogueBox->setScale(Vector2D(3, .5));
    this->attachChild(this->pDialogueBox);
    this->vecDialogue.push_back("Anything on Your Mind ?");
    this->pDialogueText = new Text("Speaker", "JainiPurva-Regular.ttf", 90, 0, false);
    this->pDialogueText->setMessage("PlaceHolder");
    this->pDialogueText->setPos(Vector2D(0,0));
    this->pDialogueText->setScale(Vector2D(1,1));
    this->pDialogueText->setColor(SDL_Color(0, 0, 0, 255));
    this->pDialogueBox->attachChild(this->pDialogueText);
    this->pDialogueBox->setPos(Vector2D(950, 900));

    DialogueToggle* pDiaTog = new DialogueToggle(EventKey::DIALOGUE_SCREEN);
    pTransBack->attachComponent(pDiaTog);

    EventBroadcaster::getInstance()->registerListener(this);
}

void DialogueBox::onEventTrigger(std::unordered_map<std::string, void*> mapParameter)
{
    bool bFromToggle = false;
    if (mapParameter.find("Sender") != mapParameter.end())
    {
        std::string senderType = *static_cast<std::string*>(mapParameter["Sender"]);
        bFromToggle = (senderType == "DialogueToggle");

    }
    if (!this->bEnabled)
    {
        this->pDialogueText->modifyText(this->vecDialogue[this->nCounter]);
        this->setEnabled(true);
        EventBroadcaster::getInstance()->disableOtherListenerExcept(this);
    }
    else if (this->bEnabled && bFromToggle)
    {
        std::cout << "progress" << std::endl;
        this->nCounter++;
        if (this->nCounter >= this->vecDialogue.size())
        {
            this->setEnabled(false);
            EventBroadcaster::getInstance()->enableAllListeners();
            this->nCounter = 0;
        }
        this->pDialogueText->modifyText(this->vecDialogue[this->nCounter]);

    }
}

EventKey DialogueBox::getKey()
{
    return this->EKey;
}

bool DialogueBox::isListenerEnabled()
{
    return this->bListenerEnabled;
}

void DialogueBox::setListenerEnabled(bool bListenerEnabled)
{
    this->bListenerEnabled = bListenerEnabled;
}

std::string DialogueBox::getListenerOwnerName()
{
    return this->strName;
}
