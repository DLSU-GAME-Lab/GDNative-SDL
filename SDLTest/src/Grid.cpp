#include "Grid.h"
#include "AGameObject.h"

Grid::Grid(std::string strName, ComponentType EType, float fWidth, float fHeight) : AComponent(strName, EType)
{
	this->fWidth = fWidth;
	this->fHeight = fHeight;
}

void Grid::onAttach()
{
	
}

// set cell object, return true on success
bool Grid::setCellObject(Uint64 r, Uint64 c, AGameObject* pGameObject)
{
	if (r >= this->gridCells.size() || c >= this->gridCells[r].size()) return false;

	this->gridCells[r][c].obj = pGameObject;
	return true;
}


// get cell world position; return empty vector on bad indices
Vector2D Grid::getCellPosition(Uint64 r, Uint64 c)
{
	return Vector2D(r * fHeight, c * fWidth) + this->pOwner->getPos();
}

// get cell object pointer; return nullptr on bad indices
AGameObject* Grid::getCellObject(Uint64 r, Uint64 c)
{
	if (r >= this->gridCells.size() || c >= this->gridCells[r].size()) return nullptr;
	return this->gridCells[r][c].obj;
}

// find cell by object pointer; returns pointer to cell data or NULL
CellData* Grid::getCellFromObject(AGameObject* pObject)
{
	for (Uint64 r = 0; r < this->gridCells.size(); r++)
	{
		for (Uint64 c = 0; c < this->gridCells[r].size(); c++)
		{
			if (this->gridCells[r][c].obj == pObject) return &this->gridCells[r][c];
		}
	}

	return nullptr;
}
