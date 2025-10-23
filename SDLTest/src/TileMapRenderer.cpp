#include "TileMapRenderer.h"
#include "AGameObject.h"

TileMapRenderer::TileMapRenderer(float fTileWidth, float fTileHeight) : ARenderer("TileMapRenderer")
{
    this->fTileWidth = fTileWidth;
    this->fTileHeight = fTileHeight;

    const Uint64 initialSize = 64;
    this->vecTile.resize(initialSize);

    for (Uint64 c = 0; c < this->vecTile.size(); c++)
        this->vecTile[c].resize(initialSize);
}

TileMapRenderer::~TileMapRenderer()
{

}

void TileMapRenderer::onAttach()
{

} 

void TileMapRenderer::perform()
{
    Camera* pCam = CameraManager::getInstance()->getCurrentCamera();

    for (Uint64 c = 0; c < this->vecTile.size(); c++)
    {
        for (Uint64 r = 0; r < this->vecTile[c].size(); r++)
        {
            if (this->vecTile[c][r] != nullptr)
            {
                SDL_FRect destRect = {};
                SDL_Texture* tile = this->vecTile[c][r];
                Vector2D scale = this->pOwner->getScale();
                Vector2D pos = this->pOwner->getPos();
                Vector2D tileSize;
                Vector2D cellSize;

                tileSize.x = tile->w * scale.x;
                tileSize.y = tile->h * scale.y;

                cellSize.x = this->fTileWidth * scale.x;
                cellSize.y = this->fTileHeight * scale.y;
                
                pos.x += cellSize.x * (c - this->offset.x);
                pos.y += cellSize.y * (r - this->offset.y);

                destRect.x = pos.x;
                destRect.y = pos.y;
                destRect.w = tileSize.x;
                destRect.h = tileSize.y;

                destRect = pCam->worldToScreenRect(destRect);

                SDL_RenderTexture(this->pRenderer, tile, nullptr, &destRect);
            }
        }
    }
}

void TileMapRenderer::addTile(Uint64 c, Uint64 r, SDL_Texture* pTile)
{
    if (this->vecTile.size() <= c) this->vecTile.resize(c + 1);
    if (this->vecTile[c].size() <= r) this->vecTile[c].resize(r + 1);
    this->vecTile[c][r] = pTile;
}

void TileMapRenderer::setTileSize(float fTileWidth, float fTileHeight)
{
    this->fTileWidth = fTileWidth;
    this->fTileHeight = fTileHeight;
}

void TileMapRenderer::setOffset(Vector2D offset)
{
    this->offset = offset;
}
