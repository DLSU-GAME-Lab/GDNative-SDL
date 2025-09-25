#pragma once
#include "AComponent.h"
#include "Vector2D.h"

#include <vector>

class Grid : public AComponent
{
private:
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
    float nCellSize;
    std::vector<CellData> vecCell;

public:
    Grid(std::string strName, Uint8 w, Uint8 h, float nCellSize);
    void perform() override;

    std::vector<AGameObject*> getAdjacentObjects(AGameObject* pGameObject, bool bIncludeDiagonals = false);

    void setBlockedCell(Uint8 r, Uint8 c, bool bBlocked);
    bool setCellObject(Uint8 r, Uint8 c, AGameObject* pGameObject);

    CellData getCellDataFromObject(AGameObject* pGameObject);
    AGameObject* getCellObject(Uint8 r, Uint8 c);
};

