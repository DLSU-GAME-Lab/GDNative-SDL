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
};

class Grid : public AComponent
{
protected:
    typedef std::vector<std::vector<CellData>> GridCells;

    float fWidth;
    float fHeight;
    GridCells gridCells;

public:
    Grid(std::string strName, ComponentType EType, float fWidth = 1.0f, float fHeight = 1.0f);

    virtual void onAttach() override;
    virtual void perform() = 0;

    bool setCellObject(Uint64 r, Uint64 c, AGameObject* pGameObject);

    Vector2D getCellPosition(CellData cell);
    Vector2D getCellPosition(Uint64 r, Uint64 c);
    AGameObject* getCellObject(Uint64 r, Uint64 c);
    CellData* getCellFromObject(AGameObject* pObject);
};

