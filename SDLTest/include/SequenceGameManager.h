#pragma once
#include "AComponent.h"
#include "Vector2D.h"
#include <vector>

class PuzzleToken;

class SequenceGameManager : public AComponent
{
private:
    std::vector<PuzzleToken*> vecToken;
    std::vector<Vector2D> vecPosition;
    std::vector<PuzzleToken*> vecTokenHolder;

public:
    void perform() override;

    void addToken(PuzzleToken* pToken);
    void removeToken(PuzzleToken* pToken);

    int getSlotIndex(PuzzleToken* pToken);
    void assignTokenSlot(PuzzleToken* pToken, int nSlot);

    /* * * * * * * * * * * * * * * * * * * * *
     *       SINGLETON-RELATED CONTENT       *
     * * * * * * * * * * * * * * * * * * * * */
private:
    static SequenceGameManager* P_SHARED_INSTANCE;

private:
    SequenceGameManager();
    SequenceGameManager(const SequenceGameManager&);
    SequenceGameManager& operator = (const SequenceGameManager&);

public:
    static SequenceGameManager* getInstance();
    static void initialize();
    static void destroy();
    /* * * * * * * * * * * * * * * * * * * * */
};

