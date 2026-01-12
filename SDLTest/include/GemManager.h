#pragma once
#include "AComponent.h"
#include "Grid.h"
#include "Gem.h"
#include <vector>

struct GemData
{
    Uint8 r;
    Uint8 c;
    Gem* gem;
    bool blocked;
};

class GemManager : public AComponent
{
private:
    typedef std::vector<std::vector<GemData>> GemData2D;

    Uint8 nWidth;
    Uint8 nHeight;
    float fGemSize;
    GemData2D data;

    GemData* pSelected[2];
    float fGemScale;
    bool bAnimating;
    std::vector<Gem*> toRemove;
    Uint64 nGemNum;

    AGameObject* pSelector;

public:
    void onAttach() override;
    void perform() override;

    static void loadResources();
    static void unloadResources();

    void setSelected(Gem* pGem);
    
    void spawnGems(float fScale);
    void updateBoard();
    void finishAnimation();

    void setBlocked(Uint8 r, Uint8 c, bool bBlocked);
    void setBlocked(Uint8 r, const std::vector<Uint8>& cols, bool bBlocked);

private:
    Vector2D getGemDataPosition(GemData gemData);
    GemData* getDataFromGem(Gem* pGem);

    void setTween(GemData gemData, Vector2D startOffset, bool bounce = false);

    bool checkMatches();
    bool checkBombs();
    void markForRemoval(Gem* pGem);
    void moveGems();
    void cascadeDown();
    void printGridData();

    std::vector<Gem*> getAdjacentGems(const GemData gemData);
    std::vector<Gem*> getAdjacentVerticalGems(const GemData gemData);
    std::vector<Gem*> getAdjacentHorizontalGems(const GemData gemData);

    /* * * * * * * * * * * * * * * * * * * * *
     *       SINGLETON-RELATED CONTENT       *
     * * * * * * * * * * * * * * * * * * * * */
private:
    static GemManager* P_SHARED_INSTANCE;

private:
    GemManager(Uint8 w, Uint8 h, float fCellSize);
    GemManager(const GemManager&);
    GemManager& operator = (const GemManager&) {};

public:
    static void initialize(Uint8 w, Uint8 h, float fCellSize, Vector2D offset);
    static void destroy();

    static GemManager* getInstance();
    /* * * * * * * * * * * * * * * * * * * * */

};

