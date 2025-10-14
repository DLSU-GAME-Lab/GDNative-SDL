#include "TileMapRenderer.h"
#include "AGameObject.h"

TileMapRenderer::TileMapRenderer(float fWidth, float fHeight) : AComponent("TileMapRenderer", ComponentType::RENDERER)
{
    this->fWidth = fWidth;
    this->fHeight = fHeight;

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
    for (Uint64 r = 0; r < this->vecTile.size(); r++)
    {
        for (Uint64 c = 0; c < this->vecTile[r].size(); c++)
        {
            if (this->vecTile[r][c] != nullptr)
            {
                SDL_FRect destRect = {};
                Vector2D scale = this->pOwner->getScale();

                destRect.x = c * this->fWidth * scale.x;
                destRect.y = r * this->fHeight * scale.y;
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
