#include "TileMapRenderer.h"
#include "AGameObject.h"

TileMapRenderer::TileMapRenderer(float fTileWidth, float fTileHeight) : ARenderer("TileMapRenderer")
{
    this->fTileWidth = fTileWidth;
    this->fTileHeight = fTileHeight;

    const Uint64 initialSize = 64;
    this->vecTile.resize(initialSize);

    for (Uint64 r = 0; r < this->vecTile.size(); r++)
        this->vecTile[r].resize(initialSize);
}

TileMapRenderer::~TileMapRenderer()
{

}

void TileMapRenderer::onAttach()
{

} 

void TileMapRenderer::perform()
{
    Camera* pCamera = CameraManager::getInstance()->getCurrentCamera();

    for (Uint64 r = 0; r < this->vecTile.size(); r++)
    {
        for (Uint64 c = 0; c < this->vecTile[r].size(); c++)
        {
            if (this->vecTile[r][c] != nullptr)
            {
                SDL_FRect destRect = {};
                Vector2D scale = this->pOwner->getScale();
                Vector2D pos = Vector2D();

                pos.x = c * this->fTileWidth * scale.x;
                pos.y = r * this->fTileHeight * scale.y;
                pos = pCamera->worldToScreenPoint(pos);

                destRect.x = pos.x;
                destRect.y = pos.y;
                destRect.w = this->vecTile[r][c]->w * scale.x;
                destRect.h = this->vecTile[r][c]->h * scale.y;

                SDL_RenderTexture(this->pRenderer, this->vecTile[r][c], nullptr, &destRect);
            }
        }
    }
}

void TileMapRenderer::addTile(Uint64 r, Uint64 c, SDL_Texture* pTile)
{
    if (this->vecTile.size() <= r) this->vecTile.resize(r + 1);
    if (this->vecTile[r].size() <= c) this->vecTile[r].resize(c + 1);

    this->vecTile[r][c] = pTile;
}

void TileMapRenderer::removeTile(Uint64 r, Uint64 c)
{
    if (this->vecTile.size() <= r || this->vecTile[r].size() <= c) return;
    this->vecTile[r][c] = nullptr;
}

void TileMapRenderer::setTileSize(float fTileWidth, float fTileHeight)
{
    this->fTileWidth = fTileWidth;
    this->fTileHeight = fTileHeight;
}
