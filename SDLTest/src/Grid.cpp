#include "Grid.h"
#include "AGameObject.h"

Grid::Grid(std::string strName, ComponentType EType, Uint64 w, Uint64 h, float fCellSize) : AComponent(strName, EType)
{
	this->nWidth = w;
	this->nHeight = h;
	this->fCellSize = fCellSize;
}

void Grid::onAttach()
{
	Vector2D cellSize = Vector2D(this->fCellSize);
	Vector2D gridSize = Vector2D(this->nWidth, this->nHeight);

	for (Uint64 r = 0; r < this->nHeight; r++)
	{
		std::vector<CellData> col;

		for (Uint64 c = 0; c < this->nWidth; c++)
		{
			Vector2D pos = cellSize * (Vector2D(c, r) - (gridSize * 0.5f));
			pos.y *= -1.0f;
			pos += pOwner->getPos();
			col.push_back({ r, c, pos, NULL, false });
		}

		this->gridCells.push_back(col);
	}
}

// set a cell's blocked flag
void Grid::setBlockedCell(Uint64 r, Uint64 c, bool bBlocked)
{
	// note: r is row index (height), c is column index (width)
	if (r >= this->nHeight || c >= this->nWidth) return;

	this->gridCells[r][c].blocked = bBlocked;
}

void Grid::setBlockedCells(Uint64 r, const std::vector<Uint64>& cols, bool bBlocked)
{
	if (cols.empty()) return;
	for (Uint64 c : cols) this->gridCells[r][c].blocked = bBlocked;
}

// set cell object, return true on success
bool Grid::setCellObject(Uint64 r, Uint64 c, AGameObject* pGameObject)
{
	if (r >= this->nHeight || c >= this->nWidth) return false;

	this->gridCells[r][c].obj = pGameObject;
	return true;
}

// get cell world position; return empty vector on bad indices
Vector2D Grid::getCellPosition(Uint64 r, Uint64 c)
{
	if (r >= this->nHeight || c >= this->nWidth) return Vector2D();
	return this->gridCells[r][c].pos;
}

// get cell object pointer; return nullptr on bad indices
AGameObject* Grid::getCellObject(Uint64 r, Uint64 c)
{
	if (r >= this->nHeight || c >= this->nWidth) return nullptr;
	return this->gridCells[r][c].obj;
}

// find cell by object pointer; returns pointer to cell data or NULL
CellData* Grid::getCellFromObject(AGameObject* pObject)
{
	for (Uint64 r = 0; r < this->nHeight; r++)
	{
		for (Uint64 c = 0; c < this->nWidth; c++)
		{
			if (this->gridCells[r][c].obj == pObject) return &this->gridCells[r][c];
		}
	}

	return NULL;
}
