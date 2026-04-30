#include "GemInputManager.h"
#include "EmptyObject.h"
#include "GameObjectManager.h"
#include "SpriteRenderer.h"
#include "DataAssetManager.h"
#include "EventBroadcaster.h"
void GemInputManager::perform()
{
    // Checks for dropped tokens (occurs when the token had been clicked and dragged).
    for (int i = 0; i < this->vecToken.size(); i++)
    {
        if (this->vecToken[i]->getDropped())
        {
			std::cout << "Token " << i << " dropped. Checking for intersection with text box." << std::endl;
            this->vecToken[i]->setDropped(false);
            SpriteRenderer* pTokenSprite = (SpriteRenderer*)this->vecToken[i]->findComponentByName("SpriteRenderer");
			if (pTokenSprite == NULL) continue;

			int nNearestBoxIndex = -1;
			float fNearestSqrMag = FLT_MAX;
            for (int j = 0; j < this->vecTextBox.size(); j++)
			{
                SpriteRenderer* pBoxSprite = (SpriteRenderer*)this->vecTextBox[j]->findComponentByName("SpriteRenderer");
                if (pBoxSprite == NULL) continue;

                // Check for intersection between token and box.
                SDL_FRect tokenRect = pTokenSprite->getRect();
                SDL_FRect boxRect = pBoxSprite->getRect();
				float distance = Vector2D(boxRect.x - tokenRect.x, boxRect.y - tokenRect.y).SqrMagnitude();
                if (
                    SDL_HasRectIntersectionFloat(&tokenRect, &boxRect) &&
                    vecTextAssigned[j] < 0 &&
                    distance < fNearestSqrMag)
                {
					nNearestBoxIndex = j;
					fNearestSqrMag = distance;
                }
            }

            if (nNearestBoxIndex != -1)
            {
                // Assign text to the token if it intersects with the box.
                this->assignText(nNearestBoxIndex, i);
            }
            else
            {
                // Reset token if it does not intersect.
                this->vecToken[i]->setPos(this->vecPosition[i]);
                //this->resetText(nullptr);
            }
        }
    }

    for (int i = 0; i < this->vecUndo.size(); i++)
	{
        if (this->vecUndo[i]->getClicked())
        {
			this->vecUndo[i]->setClicked(false);
            this->resetText(i);
        }
    }
    if (pSubmit->getClicked())
    {
        if (this->checkWinCon())
        {
            std::cout << "Win" << std::endl;
            this->pGate->gateOpen();
        }
        pSubmit->setClicked(false);
        std::unordered_map <std::string, void*> mapParam;
        std::string strName = this->getName();
        mapParam["Sender"] = static_cast<void*>(&strName);
        EventBroadcaster::getInstance()->broadcast(EventKey::GEM_INPUT_SCREEN, mapParam);
    }
}

void GemInputManager::addToken(GemInputToken* pToken)
{
	this->vecPosition.push_back(pToken->getPos());
    this->vecToken.push_back(pToken);
}

void GemInputManager::addTextBox(Sprite* pSprite)
{
	this->vecTextBox.push_back(pSprite);
}

void GemInputManager::addText(Text* pText)
{
	this->vecTextPlaceholder.push_back(pText->getMessage());
	this->vecTextAssigned.push_back(-1);
	this->vecText.push_back(pText);
}

void GemInputManager::addUndoButton(ButtonInput* pButton)
{
	this->vecUndo.push_back(pButton);
}

void GemInputManager::addSubmitButton(ButtonInput* pButton)
{
    this->pSubmit = pButton;
}

void GemInputManager::addWinCon(int nWincon)
{
    this->vecWinCon.push_back(nWincon);
}

void GemInputManager::addGate(Gate* pGate)
{
    this->pGate = pGate;
}


void GemInputManager::checkCollection()
{
	std::cout << "Checking gem collection status..." << std::endl;
    for (int i = 0; i < this->vecToken.size(); i++)
    {
        if (this->pDataAsset->getGemPickedUp(this->vecToken[i]->getName()))
        {
            this->vecToken[i]->setEnabled(true);
			std::cout << "Token " << this->vecToken[i]->getName() << " enabled." << std::endl;
        }
        else
        {
            this->vecToken[i]->setEnabled(false);
        }
	}
}

void GemInputManager::assignText(int nTextIndex, int nTokenIndex)
{
	this->vecToken[nTokenIndex]->setEnabled(false);
	this->vecText[nTextIndex]->setMessage(this->pDataAsset->getGemText(this->vecToken[nTokenIndex]->getName()));
	this->vecText[nTextIndex]->setColor({ 255, 255, 255, 255 });
	this->vecTextAssigned[nTextIndex] = nTokenIndex;
}

void GemInputManager::resetText(int nTextIndex)
{
	int nTokenIndex = this->vecTextAssigned[nTextIndex];
	if (nTokenIndex < 0) return;

    this->vecToken[nTokenIndex]->setEnabled(true);
	this->vecToken[nTokenIndex]->setPos(this->vecPosition[nTokenIndex]);
    this->vecText[nTextIndex]->setMessage(this->vecTextPlaceholder[nTextIndex]);
    this->vecText[nTextIndex]->setColor({ 255, 255, 255, 127 });
    this->vecTextAssigned[nTextIndex] = -1;
}

void GemInputManager::resetAllText()
{
    for (int i = 0; i < this->vecTextAssigned.size(); i++)
    {
        this->resetText(i);
    }
}

bool GemInputManager::checkWinCon()
{
    for (int i = 0; i < this->vecTextAssigned.size(); i++)
    {
        if (this->vecTextAssigned[i] != this->vecWinCon[i])
            return false;
    }
    return true;
}

GemInputManager* GemInputManager::P_SHARED_INSTANCE = NULL;

GemInputManager::GemInputManager() : AComponent("GemInputManager", ComponentType::SCRIPT)
{
	this->pDataAsset = (CollectableGemDataAsset*)DataAssetManager::getInstance()->getDataAsset("CollectableGemDataAsset");

}

void GemInputManager::initialize()
{
    EmptyObject* pManagerObject = new EmptyObject("GemInputManager");
    P_SHARED_INSTANCE = new GemInputManager();
    pManagerObject->attachComponent(P_SHARED_INSTANCE);
    GameObjectManager::getInstance()->addObject(pManagerObject);
}

void GemInputManager::destroy()
{
    delete P_SHARED_INSTANCE->pOwner;
}

GemInputManager* GemInputManager::getInstance()
{
    return P_SHARED_INSTANCE;
}
