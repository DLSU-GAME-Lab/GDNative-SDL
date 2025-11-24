#include "TileMapRenderer.h"
#include "AGameObject.h"

TileMapRenderer::TileMapRenderer(float fTileWidth, float fTileHeight) : ARenderer("TileMapRenderer")
{
    this->fTileWidth = fTileWidth;
    this->fTileHeight = fTileHeight;
    this->offset = Vector2D(0.5f);

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
                Vector2D tilePos = this->pOwner->getPos();
                Vector2D cellSize = this->getCellSize();
                Vector2D tileSize;

                tileSize.x = tile->w * scale.x;
                tileSize.y = tile->h * scale.y;

                tilePos.x += (cellSize.x * (c + 0.5f)) - (tileSize.x * this->offset.x);
                tilePos.y += (cellSize.y * (r + 0.5f)) - (tileSize.y * this->offset.y);

                destRect.x = tilePos.x;
                destRect.y = tilePos.y;
                destRect.w = tileSize.x;
                destRect.h = tileSize.y;

                destRect = pCam->worldToScreenRect(destRect);

                if (this->inCameraView(destRect))
                    SDL_RenderTexture(this->pRenderer, tile, nullptr, &destRect);
            }
        }
    }
}

void TileMapRenderer::drawWidget()
{
    Camera* pCam = CameraManager::getInstance()->getCurrentCamera();

    for (Uint64 c = 0; c < this->vecTile.size(); c++)
    {
        for (Uint64 r = 0; r < this->vecTile[c].size(); r++)
        {
            SDL_FRect destRect = {};
            Vector2D cellPos = this->pOwner->getPos();
            Vector2D cellSize = this->getCellSize();

            cellPos.x += cellSize.x * c;
            cellPos.y += cellSize.y * r;

            destRect.x = cellPos.x;
            destRect.y = cellPos.y;
            destRect.w = cellSize.x;
            destRect.h = cellSize.y;

            destRect = pCam->worldToScreenRect(destRect);
            if (this->inCameraView(destRect))
            {
                SDL_SetRenderDrawBlendMode(this->pRenderer, SDL_BLENDMODE_BLEND);
                SDL_SetRenderDrawColor(this->pRenderer, 127, 127, 127, 63);
                SDL_RenderRect(this->pRenderer, &destRect);
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

Vector2D TileMapRenderer::getCellSize()
{
    Vector2D cellSize;
    cellSize.x = this->fTileWidth * this->pOwner->getScale().x;
    cellSize.y = this->fTileHeight * this->pOwner->getScale().y;

    return cellSize;
}

Vector2D TileMapRenderer::getTilePosition(Uint64 c, Uint64 r, bool clamp)
{
    Vector2D pos = this->pOwner->getPos();
    Vector2D cellSize = this->getCellSize();
    pos.x += cellSize.x * (c + 0.5f);
    pos.y += cellSize.y * (r + 0.5f);

    if (clamp)
    {
        pos.x = (int)pos.x;
        pos.y = (int)pos.y;
    }

    return pos;
}
