#include "BubbleManager.h"
#include "GameObjectManager.h"
#include "EmptyObject.h"
#include "Bubble.h"

void BubbleManager::perform()
{
    this->fTicks += fDeltaTime;
    float spawnDelay = this->getRandRange(this->fMinSpawnDelay, this->fMaxSpawnDelay);
    if (this->fTicks > spawnDelay)
    {
        this->fTicks = 0.0f;
        Uint16 spawnCount = 0;
        while (this->vecInactiveBubbles.size() != 0 && spawnCount != this->nSpawnCount)
        {
            int index = this->vecInactiveBubbles.size() - 1;
            Bubble* pBubble = this->vecInactiveBubbles[index];
            pBubble->setPos(Vector2D(3000.0f, 3000.0f));
            float scale = this->getRandRange(this->fMinScale, this->fMaxScale);
            pBubble->setScale(Vector2D(scale));
            pBubble->randomizeTween();
            pBubble->setEnabled(true);

            this->vecActiveBubbles.push_back(pBubble);
            this->vecInactiveBubbles.erase(this->vecInactiveBubbles.begin() + index);
            spawnCount++;
        }
    }
}

void BubbleManager::deactivateBubble(Bubble* pBubble)
{
    int nIndex = -1;

    for (int i = 0; i < this->vecActiveBubbles.size() && nIndex == -1; i++)
    {
        if (this->vecActiveBubbles[i] == pBubble) nIndex = i;
    }

    if (nIndex != -1)
    {
        this->vecActiveBubbles[nIndex]->setEnabled(false);
        this->vecInactiveBubbles.push_back(this->vecActiveBubbles[nIndex]);
        this->vecActiveBubbles.erase(this->vecActiveBubbles.begin() + nIndex);
    }
}

float BubbleManager::getRandRange(float min, float max)
{
    return ((max - min) * ((float)rand() / RAND_MAX)) + min;
}

BubbleManager* BubbleManager::P_SHARED_INSTANCE = NULL;

BubbleManager::BubbleManager(unsigned int nMaxBubbles) : AComponent("BubbleManager", ComponentType::SCRIPT)
{
    this->nMaxBubbles = nMaxBubbles;
    for (int i = 0; i < this->nMaxBubbles; i++)
    {
        Bubble* pBubble = new Bubble("Bubble_" + std::to_string(i));
        pBubble->setPos(Vector2D(3000.0f, 3000.0f));
        GameObjectManager::getInstance()->addObject(pBubble);
        this->vecInactiveBubbles.push_back(pBubble);
        pBubble->setEnabled(false);
    }

    this->fTicks = 0.0f;
}

void BubbleManager::initialize(unsigned int nMaxBubbles)
{
    P_SHARED_INSTANCE = new BubbleManager(nMaxBubbles);
    EmptyObject* pManager = new EmptyObject("BubbleManager");
    pManager->attachComponent(P_SHARED_INSTANCE);
    GameObjectManager::getInstance()->addObject(pManager);
}

void BubbleManager::destroy()
{
    delete P_SHARED_INSTANCE->pOwner;
}

BubbleManager* BubbleManager::getInstance()
{
    return P_SHARED_INSTANCE;
}
