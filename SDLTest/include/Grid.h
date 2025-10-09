#pragma once
#include "AComponent.h"
#include "Vector2D.h"

#include <vector>
#include <unordered_map>

struct CellData
{
    Uint64 r;
    Uint64 c;
    AGameObject* obj;
    bool blocked;
};

class Grid : public AComponent
{
protected:
    typedef std::vector<std::vector<CellData>> GridCells;

    Uint64 nWidth;
    Uint64 nHeight;
    float fCellSize;
    GridCells gridCells;

public:
    Grid(std::string strName, ComponentType EType, Uint64 w, Uint64 h, float nCellSize);

    virtual void onAttach() override;
    virtual void perform() = 0;

    void setBlockedCell(Uint64 r, Uint64 c, bool bBlocked);
    void setBlockedCells(Uint64 r, const std::vector<Uint64>& cols, bool bBlocked);
    bool setCellObject(Uint64 r, Uint64 c, AGameObject* pGameObject);

    Vector2D getCellPosition(CellData cell);
    Vector2D getCellPosition(Uint64 r, Uint64 c);
    AGameObject* getCellObject(Uint64 r, Uint64 c);
    CellData* getCellFromObject(AGameObject* pObject);
};

