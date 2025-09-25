#include "Grid.h"
#include "AGameObject.h"

Grid::Grid(std::string strName, Uint8 w, Uint8 h, float fCellSize) : AComponent(strName, ComponentType::SCRIPT)
{
	this->nWidth = w;
	this->nHeight = h;
	this->fCellSize = fCellSize;
}

void Grid::setup()
{

	Vector2D cellSize = Vector2D(this->fCellSize);
	Vector2D gridSize = Vector2D(this->nWidth, this->nHeight);

	for (Uint8 r = 0; r < this->nHeight; r++)
	{
		for (Uint8 c = 0; c < this->nWidth; c++)
		{
			Vector2D pos = cellSize * (Vector2D(c, r) - (gridSize * 0.5f));
			pos.y *= -1.0f;
			pos += pOwner->getPos();
			this->vecCell.push_back({ r, c, pos, NULL, false });
		}
	}
}

void Grid::perform()
{

}

std::vector<AGameObject*> Grid::getAdjacentObjects(AGameObject* pGameObject, bool bIncludeDiagonals)
{
	std::vector<AGameObject*> vecAdjacent;
	CellData cell = getCellDataFromObject(pGameObject);

	for (Uint8 c = 0; c < 3; c++)
	{
		for (Uint8 r = 0; r < 3; r++)
		{
			int row = cell.r - r;
			int col = cell.c = c;

			if (row < 0 || row >= this->nWidth ||
				col < 0 || col >= this->nHeight)
			{
				continue;
			}
			else
			{
				vecAdjacent.push_back(getCellObject(row, col));
			}
		}
	}

	return vecAdjacent;
}

void Grid::setBlockedCell(Uint8 r, Uint8 c, bool bBlocked)
{
	if (r >= this->nWidth || c >= this->nHeight) return;

	for (CellData& cell : this->vecCell)
	{
		if (cell.r == r && cell.c == c)
		{
			cell.blocked = bBlocked;
			return;
		}
	}
}

bool Grid::setCellObject(Uint8 r, Uint8 c, AGameObject* pGameObject)
{
	if (r >= this->nWidth || c >= this->nHeight) return false;

	for (CellData& cell : this->vecCell)
	{
		if (cell.r == r && cell.c == c)
		{
			if (cell.blocked) return false;
			else
			{
				cell.obj = pGameObject;
				return true;
			}
		}
	}

	return false;
}

std::vector<Grid::CellData>& Grid::getAllCellData()
{
	return this->vecCell;
}

Grid::CellData Grid::getCellDataFromObject(AGameObject* pGameObject)
{
	for (const CellData& cell : this->vecCell)
	{
		if (cell.obj == pGameObject) return cell;
	}
}

Vector2D Grid::getCellPosition(Uint8 r, Uint8 c)
{
	for (const CellData& cell : this->vecCell)
	{
		if (cell.r == r && cell.c == c) return cell.pos;
	}

	return Vector2D();
}

AGameObject* Grid::getCellObject(Uint8 r, Uint8 c)
{
	if (r >= this->nWidth || c >= this->nHeight) return nullptr;

	for (const CellData& cell : this->vecCell)
	{
		if (cell.r == r && cell.c == c) return cell.obj;
	}

	return nullptr;
}
