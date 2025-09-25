#pragma once
#include "AComponent.h"
#include "Vector2D.h"

#include <vector>

class Grid : public AComponent
{
protected:
    struct CellData
    {
        Uint8 r;
        Uint8 c;
        Vector2D pos;
        AGameObject* obj;
        bool blocked;
    };

    Uint8 nWidth;
    Uint8 nHeight;
    float fCellSize;
    std::vector<CellData> vecCell;

public:
    Grid(std::string strName, Uint8 w, Uint8 h, float nCellSize);

    void setup();
    void perform() override;

    std::vector<AGameObject*> getAdjacentObjects(AGameObject* pGameObject, bool bIncludeDiagonals = false);

    void setBlockedCell(Uint8 r, Uint8 c, bool bBlocked);
    bool setCellObject(Uint8 r, Uint8 c, AGameObject* pGameObject);

    std::vector<CellData>& getAllCellData();
    CellData getCellDataFromObject(AGameObject* pGameObject);
    Vector2D getCellPosition(Uint8 r, Uint8 c);
    AGameObject* getCellObject(Uint8 r, Uint8 c);
};

