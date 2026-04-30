#include "SequenceGameManager.h"
#include "GameObjectManager.h"
#include "EmptyObject.h"
#include "PuzzleToken.h"
#include "EventBroadcaster.h"
void SequenceGameManager::perform()
{
    // Checks for dropped tokens (occurs when the token had been clicked and dragged).
    for (auto pToken : this->vecToken)
    {
        if (pToken->getDropped())
        {
            pToken->setDropped(false);
            int nearPosIndex = this->getSlotIndex(pToken);

            // Reassign the slot of the other token if the slot is occupied.
            if (this->vecTokenHolder[nearPosIndex] != NULL)
            {
                PuzzleToken* pOther = this->vecTokenHolder[nearPosIndex];
                this->assignTokenSlot(pOther, pToken->getSlot());
            }
            // Move the token to the slot.
            this->vecTokenHolder[pToken->getSlot()] = NULL;
            this->assignTokenSlot(pToken, nearPosIndex);
        }
    }
}

void SequenceGameManager::addToken(PuzzleToken* pToken)
{
    this->vecToken.push_back(pToken);
    this->assignTokenSlot(pToken, this->getSlotIndex(pToken));
}

void SequenceGameManager::removeToken(PuzzleToken* pToken)
{
    int nIndex = -1;

    for (int i = 0; i < this->vecToken.size() && nIndex == -1; i++)
    {
        if (this->vecToken[i] == pToken)
            nIndex = i;
    }

    if (nIndex != -1)
    {
        this->vecToken.erase(this->vecToken.begin() + nIndex);
        delete pToken;
    }
}

void SequenceGameManager::setCombination(std::string strFirst, std::string strSecond, std::string strThird)
{
    this->mapCombi[3] = strFirst;
    this->mapCombi[4] = strSecond;
    this->mapCombi[5] = strThird;
}

// Check the token's slot by getting the nearest slot position.
int SequenceGameManager::getSlotIndex(PuzzleToken* pToken)
{
    float minSqrMag = 10000000.0f;
    int nearPosIndex = 0;

    for (int i = 0; i < this->vecPosition.size(); i++)
    {
        float sqrMag = (pToken->getPos() - this->vecPosition[i]).SqrMagnitude();
        if (sqrMag < minSqrMag)
        {
            minSqrMag = sqrMag;
            nearPosIndex = i;
        }
    }

    return nearPosIndex;
}

void SequenceGameManager::assignTokenSlot(PuzzleToken* pToken, int nSlot)
{
    pToken->setSlot(nSlot);
    pToken->setPos(this->vecPosition[nSlot]);
    this->vecTokenHolder[nSlot] = pToken;
    this->checkToken(nSlot,pToken->getName());
    this->checkCombination();
}

void SequenceGameManager::checkToken(int nSlot, std::string strTokenName)
{
    if(nSlot > 2)
    {
        if (this->mapCombi[nSlot] == strTokenName)
        {
            this->vecCombinationCheck[nSlot - 3] = true;
        }
        else
        {
            this->vecCombinationCheck[nSlot - 3] = false;
        }
    }
}

void SequenceGameManager::checkCombination()
{
    bool bEndCheck = true;
    for (bool bCheck : this->vecCombinationCheck)
    {
        if (!bCheck)
        {
            bEndCheck = false;
            break;
        }
    }
    if (bEndCheck)
    {
        for (PuzzleToken* pToken : this->vecToken)
        {
            pToken->setEnabled(false);
        }
        EventBroadcaster::getInstance()->broadcast(EventKey::END_LEVEL);
    }
}

SequenceGameManager* SequenceGameManager::P_SHARED_INSTANCE = NULL;

SequenceGameManager::SequenceGameManager() : AComponent("SequenceGameManager", ComponentType::SCRIPT)
{
    // Initial token positions
    this->vecPosition.push_back(Vector2D(-280.0f, -375.0f));
    this->vecPosition.push_back(Vector2D(0.0f, -375.0f));
    this->vecPosition.push_back(Vector2D(280.0f, -375.0f));

    // Final token positions
    this->vecPosition.push_back(Vector2D(-380.0f, 27.0f));
    this->mapCombi[this->vecPosition.size() - 1];
    this->vecPosition.push_back(Vector2D(0.0f, 27.0f));
    this->mapCombi[this->vecPosition.size() - 1];
    this->vecPosition.push_back(Vector2D(380.0f, 27.0f));
    this->mapCombi[this->vecPosition.size() - 1];
    for (auto& entry : mapCombi) {
        std::cout << entry.first << std::endl;
    }
    this->vecCombinationCheck.push_back(false);
    this->vecCombinationCheck.push_back(false);
    this->vecCombinationCheck.push_back(false);
    // Holds the tokens; used for position swapping by matching indices with vecPosition
    for (int i = 0; i < this->vecPosition.size(); i++)
        this->vecTokenHolder.push_back(NULL);
}

void SequenceGameManager::initialize()
{
    EmptyObject* pManagerObject = new EmptyObject("SequenceGameManager");
    P_SHARED_INSTANCE = new SequenceGameManager();
    pManagerObject->attachComponent(P_SHARED_INSTANCE);
    GameObjectManager::getInstance()->addObject(pManagerObject);
}

void SequenceGameManager::destroy()
{
    delete P_SHARED_INSTANCE->pOwner;
}

SequenceGameManager* SequenceGameManager::getInstance()
{
    return P_SHARED_INSTANCE;
}
