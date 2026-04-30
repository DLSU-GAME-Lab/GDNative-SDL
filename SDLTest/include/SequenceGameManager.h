#pragma once
#include "AComponent.h"
#include "Vector2D.h"
#include <vector>
#include "unordered_map"

class PuzzleToken;

class SequenceGameManager : public AComponent
{
private:
    std::vector<PuzzleToken*> vecToken;
    std::vector<Vector2D> vecPosition;
    std::vector<PuzzleToken*> vecTokenHolder;
    std::unordered_map<int, std::string> mapCombi;
    std::vector<bool> vecCombinationCheck;

public:
    void perform() override;

    void addToken(PuzzleToken* pToken);
    void removeToken(PuzzleToken* pToken);
    void setCombination(std::string strFirst, std::string strSecond, std::string strThird);
    int getSlotIndex(PuzzleToken* pToken);
    void assignTokenSlot(PuzzleToken* pToken, int nSlot);
    void checkToken(int nSlot, std::string strTokenName);
    void checkCombination();

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

