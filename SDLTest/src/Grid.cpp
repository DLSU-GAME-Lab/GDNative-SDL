#include "Grid.h"
#include "AGameObject.h"

Grid::Grid(std::string strName, Uint8 w, Uint8 h, float fCellSize) : AComponent(strName, ComponentType::SCRIPT)
{
	this->nWidth = w;
	this->nHeight = h;
	this->fCellSize = fCellSize;
}

void Grid::onAttach()
{
	Vector2D cellSize = Vector2D(this->fCellSize);
	Vector2D gridSize = Vector2D(this->nWidth, this->nHeight);

	for (Uint8 r = 0; r < this->nHeight; r++)
	{
		std::vector<CellData> col;
		for (Uint8 c = 0; c < this->nWidth; c++)
		{
			Vector2D pos = cellSize * (Vector2D(c, r) - (gridSize * 0.5f));
			pos.y *= -1.0f;
			pos += pOwner->getPos();
			col.push_back({ r, c, pos, NULL, false });
		}
		this->gridCells.push_back(col);
	}
}

void Grid::perform()
{

}

// set a cell's blocked flag
void Grid::setBlockedCell(Uint8 r, Uint8 c, bool bBlocked)
{
	// note: r is row index (height), c is column index (width)
	if (r >= this->nHeight || c >= this->nWidth) return;

	this->gridCells[r][c].blocked = bBlocked;
}

void Grid::setBlockedCells(Uint8 r, const std::vector<Uint8>& cols, bool bBlocked)
{
	if (cols.empty()) return;
	for (Uint8 c : cols) this->gridCells[r][c].blocked = bBlocked;
}

// set cell object, return true on success
bool Grid::setCellObject(Uint8 r, Uint8 c, AGameObject* pGameObject)
{
	if (r >= this->nHeight || c >= this->nWidth) return false;

	this->gridCells[r][c].obj = pGameObject;
	return true;
}

// get cell world position; return empty vector on bad indices
Vector2D Grid::getCellPosition(Uint8 r, Uint8 c)
{
	if (r >= this->nHeight || c >= this->nWidth) return Vector2D();
	return this->gridCells[r][c].pos;
}

// get cell object pointer; return nullptr on bad indices
AGameObject* Grid::getCellObject(Uint8 r, Uint8 c)
{
	if (r >= this->nHeight || c >= this->nWidth) return nullptr;
	return this->gridCells[r][c].obj;
}

// find cell by object pointer; returns pointer to cell data or NULL
CellData* Grid::getCellFromObject(AGameObject* pObject)
{
	for (Uint8 r = 0; r < this->nHeight; r++)
	{
		for (Uint8 c = 0; c < this->nWidth; c++)
		{
			if (this->gridCells[r][c].obj == pObject) return &this->gridCells[r][c];
		}
	}

	return NULL;
}
