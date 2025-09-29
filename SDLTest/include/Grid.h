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

    typedef std::vector<std::vector<CellData>> GridCells;

    Uint8 nWidth;
    Uint8 nHeight;
    float fCellSize;
    GridCells gridCells;

public:
    Grid(std::string strName, Uint8 w, Uint8 h, float nCellSize);

    virtual void onAttach() override;
    virtual void perform() override;

    void setBlockedCell(Uint8 r, Uint8 c, bool bBlocked);
    bool setCellObject(Uint8 r, Uint8 c, AGameObject* pGameObject);
    Vector2D getCellPosition(Uint8 r, Uint8 c);
    AGameObject* getCellObject(Uint8 r, Uint8 c);
    CellData* getCellFromObject(AGameObject* pObject);
};

