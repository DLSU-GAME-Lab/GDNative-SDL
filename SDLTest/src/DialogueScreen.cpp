#include "DialogueScreen.h"
#include "Background.h"
#include "SpriteRenderer.h"
#include "ButtonInput.h"
#include "EventBroadcaster.h"
#include "DialogueToggle.h"

DialogueScreen::DialogueScreen(std::string strName):AGameObject(strName)
{
	this->pDialogueSprite = NULL;
    this->bListenerEnabled = true;
    this->EKey = EventKey::DIALOGUE_SCREEN;
	this->strStartBranchKey = "intro";
	this->strBranchKey = this->strStartBranchKey;
    this->bIsScreenObject = true;
}

DialogueScreen::~DialogueScreen()
{
}

void DialogueScreen::initialize()
{
    Background* pTransBack = new Background("TransparentBG", "Square", Vector2D(5.f));
    this->attachChild(pTransBack);
    SpriteRenderer* pTransBGR = (SpriteRenderer*)pTransBack->findComponentByName("SpriteRenderer");
    pTransBGR->setColor({ 0, 0, 0, 127 });
    ButtonInput* pBackgroundInput = new ButtonInput(pTransBGR);
    pTransBack->attachComponent(pBackgroundInput);

    this->pDialogueSprite = new Sprite("DialogueSprite", "librarian_default");
    this->pDialogueSprite->setIsScreenObject(true);
    this->pDialogueSprite->setPos(Vector2D(960, 540));
    this->attachChild(this->pDialogueSprite);

    this->setupButtons();
    this->setupDialogueBranches();
    EventBroadcaster::getInstance()->registerListener(this);
}

void DialogueScreen::setupButtons()
{
    for (int i = 0; i < 4; i++)
    {
        std::string strNum = std::to_string(i);
        std::string buttonName = "DialogueButton_";
        GUIButton* pDialogueButton = new GUIButton(buttonName + strNum, "Square");
        pDialogueButton->setIsScreenObject(true);
        this->attachChild(pDialogueButton);
        pDialogueButton->setScale(Vector2D(3, .5));
        pDialogueButton->setPos(Vector2D(960, 900));
        pDialogueButton->setColor({ 0, 0, 0, 255 });
		this->vecButton.push_back(pDialogueButton);

        DialogueToggle* pDiaTog = new DialogueToggle(i);
        pDialogueButton->attachComponent(pDiaTog);

        std::string spriteName = "ButtonSquare_";
        Sprite* pButtonSprite = new Sprite(spriteName + strNum, "Square");
        pButtonSprite->setIsScreenObject(true);
        pDialogueButton->attachChild(pButtonSprite);
        pButtonSprite->setScale(Vector2D(2.8, .45));
        pButtonSprite->setPos(Vector2D(960, 900));

        std::string textName = "DialogueText_";
        Text* pDialogueText = new Text(textName + strNum, "JainiPurva-Regular.ttf", 48, 0, false);
        pDialogueText->setIsScreenObject(true);
        pDialogueText->setColor(SDL_Color(0, 0, 0, 255));
        pDialogueButton->attachChild(pDialogueText);
        pDialogueText->setPos(Vector2D(960, 900));
        pDialogueText->setScale(Vector2D(1.0f));
        pDialogueText->setMessage("PlaceHolder");
		this->vecText.push_back(pDialogueText);
        
    }
}

void DialogueScreen::setupDialogueBranches()
{
	// Intro branch
    this->mapDialogueBranch["intro"] = {
        "librarian_default",
        {
            {
                "options",
                "Anything on Your Mind?"
            }
        }
    };

    // Options branch
    this->mapDialogueBranch["options"] = {
        "librarian_default",
        {
            {
                "read_0",
                "Paano ba magbasa?\nHindi ba nakakatamad?"
            },
            {
                "tips_0",
                "Any tips?"
            },
            {
                "angry_0",
                "Iniinis ako ng diwata:("
            },
            {
                "end",
                "Never mind."
            }
        }
    };

    // Read branch
    this->mapDialogueBranch["read_0"] = {
        "librarian_smile",
        {
            {
            "read_1",
            "Cliche, but it's all about imagination."
            }
        }
	};
    this->mapDialogueBranch["read_1"] = {
        "librarian_smile",
        {
            {
                "read_2",
                "Work is boring, but you can make it fun, no?"
            }
        }
    };
    this->mapDialogueBranch["read_2"] = {
        "librarian_default",
        {
            {
                "read_3",
                "In your case, I always love to insert myself into stories while reading."
            }
        }
    };
    this->mapDialogueBranch["read_3"] = {
        "librarian_pose",
        {
            {
                "read_4",
                "Maybe you could even imagine yourself meeting with the characters?"
            }
        }
    };
    this->mapDialogueBranch["read_4"] = {
        "librarian_pose",
        {
            {
                "read_5",
                "or even imagine yourself living in that world.",
            }
        }
    };
    this->mapDialogueBranch["read_5"] = {
        "librarian_default",
        {
            {
                "read_6",
                "Who knows, you could be your own protagonist?"
            }
        }
    };
    this->mapDialogueBranch["read_6"] = {
        "librarian_smile",
        {
            {
                "read_7",
                "And I'm sure our little friend has her own stories too."
            }
        }
    };
    this->mapDialogueBranch["read_7"] = {
        "librarian_smile",
        {
            {
                "exit",
                "[Like how she adores writing romance novels... haha.]"
            }
        }
    };

	// Tips branch
    this->mapDialogueBranch["tips_0"] = {
        "librarian_default",
        {
            {
            "tips_1",
			"Try to note down the important events while reading."
            }
        }
    };
    this->mapDialogueBranch["tips_1"] = {
        "librarian_hand",
        {
            {
            "tips_2",
            "Ask yourself: Who? What [happened]? Where [did it happen]? Why [did it \n happen]? How [did it happen]? (A lot of happenings there!)"
            }
        }
	};
    this->mapDialogueBranch["tips_2"] = {
        "librarian_smile",
        {
            {
            "tips_3",
            "Stories are very predictable, after all! it all comes down to experience."
            }
        }
	};
    this->mapDialogueBranch["tips_3"] = {
        "librarian_default",
        {
            {
            "tips_4",
			"The more you read, the more you can easily identify the important events."
            }
        }
	};
    this->mapDialogueBranch["tips_4"] = {
        "librarian_default",
        {
            {
            "tips_5",
            "Don't get discouraged if you don't get it the first try."
            }
		}
	};
    this->mapDialogueBranch["tips_5"] = {
        "librarian_default",
        {
            {
                "exit",
                "Just practice, alright?"
            }
        }
	};

	// Angry branch
	this->mapDialogueBranch["angry_0"] = {
        "librarian_default",
        {
            {
                "angry_1",
                "Ano po yun?"
            }
        }
    };
    this->mapDialogueBranch["angry_1"] = {
        "librarian_pose",
        {
            {
                "angry_2",
                "Oh, um, let's not put it that way, hehe."
            }
        }
	};
    this->mapDialogueBranch["angry_2"] = {
        "librarian_smile",
        {
            {
                "angry_3",
                "Yes, I admit she's pretty [nosy] but..."
            }
        }
    };
    this->mapDialogueBranch["angry_3"] = {
        "librarian_smile",
        {
            {
                "angry_4",
                "she has good intentions."
            }
        }
    };
    this->mapDialogueBranch["angry_4"] = {
        "librarian_default",
        {
            {
                "angry_5",
                "She's the reason why this library is well-kept."
            }
        }
    };
    this->mapDialogueBranch["angry_5"] = {
        "librarian_default",
        {
            {
                "exit",
                "So Just take it as a sign of her appreciation, you know?"
            }
        }
	};

	// End branch
    this->mapDialogueBranch["end"] = {
        "librarian_default",
        {
            {
                "exit",
                "Alright, I'm always here if you need me."
            }
        }
    };
}

void DialogueScreen::onEventTrigger(std::unordered_map<std::string, void*> mapParameter)
{
    if (!this->bEnabled &&
        mapParameter.find("Sender") != mapParameter.end())
    {
        this->strBranchKey = this->strStartBranchKey;
        this->setDialogue();
        this->setEnabled(true);
        EventBroadcaster::getInstance()->disableOtherListenerExcept(this);
    }
    else if (this->bEnabled &&
        mapParameter.find("DialogueToggle") != mapParameter.end())
    {
        unsigned int nDialogueKey = *static_cast<unsigned int*>(mapParameter["DialogueToggle"]);
        this->strBranchKey = this->mapDialogueBranch[this->strBranchKey].options[nDialogueKey].nextKey;
        if (this->strBranchKey == "exit")
        {
            this->setEnabled(false);
            EventBroadcaster::getInstance()->enableAllListeners();
        }
        else
        {
            setDialogue();
        }
    }
}

EventKey DialogueScreen::getKey()
{
    return this->EKey;
}

bool DialogueScreen::isListenerEnabled()
{
    return this->bListenerEnabled;
}

void DialogueScreen::setListenerEnabled(bool bListenerEnabled)
{
    this->bListenerEnabled = bListenerEnabled;
}

std::string DialogueScreen::getListenerOwnerName()
{
    return this->strName;
}

void DialogueScreen::setDialogue()
{
	const DialogueBranch& branch = this->mapDialogueBranch[this->strBranchKey];
    this->pDialogueSprite->setImage(branch.sprite);

    for (int i = 0; i < this->vecButton.size(); i++)
    {
        this->vecButton[i]->setEnabled(false);
        this->vecText[i]->setEnabled(false);
	}

	const float fButtonSpacing = 470.0f;
	const float fY = 900.0f;
    const int size = branch.options.size();
    for (int i = 0; i < size; i++)
    {
		Vector2D buttonScale = Vector2D(3.3f / size, .5);
        Vector2D buttonPos = Vector2D(960 + (i - (size - 1) / 2.0f) * fButtonSpacing, fY);
        
        this->vecButton[i]->setPos(buttonPos);
		this->vecButton[i]->setScale(buttonScale);
        this->vecButton[i]->setEnabled(true);

        this->vecText[i]->setPos(buttonPos);
		this->vecText[i]->setScale(Vector2D(1.0f));
        this->vecText[i]->setMessage(branch.options[i].text);
        this->vecText[i]->setEnabled(true);
    }
}
