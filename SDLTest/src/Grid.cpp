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

void Grid::setBlockedCell(Uint8 r, Uint8 c, bool bBlocked)
{
	if (r >= this->nWidth || c >= this->nHeight) return;

	this->gridCells[r][c].blocked = bBlocked;
}

bool Grid::setCellObject(Uint8 r, Uint8 c, AGameObject* pGameObject)
{
	if (r >= this->nWidth || c >= this->nHeight) return false;

	this->gridCells[r][c].obj = pGameObject;

	return false;
}

Vector2D Grid::getCellPosition(Uint8 r, Uint8 c)
{
	if (r >= this->nWidth || c >= this->nHeight) return Vector2D();
	else this->gridCells[r][c].pos;
}

AGameObject* Grid::getCellObject(Uint8 r, Uint8 c)
{
	if (r >= this->nWidth || c >= this->nHeight) return nullptr;
	else this->gridCells[r][c].obj;
}

Grid::CellData* Grid::getCellFromObject(AGameObject* pObject)
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
