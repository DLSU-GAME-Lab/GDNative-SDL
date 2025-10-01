#pragma once
#include "AComponent.h"
#include "Grid.h"
#include "Gem.h"
#include <vector>

class GemManager : public Grid
{
private:
    Gem* pSelectedObjects[2];
    CellData* pSelectedCells[2];
    float fGemSize;

public:
    void onAttach() override;
    void perform() override;

    static void loadResources();
    static void unloadResources();

    void setSelected(Gem* pSelected);
    void spawnGems(float fScale);

private:
    void moveGems();
    bool checkMatches();
    void cascadeDown();
    void printGridData();

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

