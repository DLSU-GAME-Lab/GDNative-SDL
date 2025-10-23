#pragma once
#include "AComponent.h"
#include <vector>

class Bubble;

class BubbleManager : public AComponent
{
private:
    Uint16 nMaxBubbles;
    std::vector<Bubble*> vecActiveBubbles;
    std::vector<Bubble*> vecInactiveBubbles;

    const Uint16 nSpawnCount = 3;
    const float fMinSpawnDelay = 0.5f;
    const float fMaxSpawnDelay = 3.0f;
    const float fMinScale = 0.5f;
    const float fMaxScale = 1.0f;
    float fTicks;

public:
    void perform() override;

    void deactivateBubble(Bubble* pBubble);

private:
    float getRandRange(float min, float max);

    /* * * * * * * * * * * * * * * * * * * * *
     *       SINGLETON-RELATED CONTENT       *
     * * * * * * * * * * * * * * * * * * * * */
private:
    static BubbleManager* P_SHARED_INSTANCE;

private:
    BubbleManager(unsigned int nMaxBubbles);
    BubbleManager(const BubbleManager&);
    BubbleManager& operator = (const BubbleManager&) {};

public:
    static void initialize(unsigned int nMaxBubbles);
    static void destroy();

    static BubbleManager* getInstance();
    /* * * * * * * * * * * * * * * * * * * * */

};

