#include "TileMapRenderer.h"

TileMapRenderer::TileMapRenderer(Uint64 w, Uint64 h, float fCellSize)
	: Grid("TileMapRenderer", ComponentType::RENDERER, w, h, fCellSize)
{

}

TileMapRenderer::~TileMapRenderer()
{

}

void TileMapRenderer::onAttach()
{
	Grid::onAttach();
}

void TileMapRenderer::perform()
{
	
}
