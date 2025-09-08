#include <iostream>
#include "SpriteRenderer.h"
#include "TextureManager.h"
#include "SpriteRendererSystem.h"

SpriteRenderer::SpriteRenderer(const std::string& textureName, float x, float y, float w, float h)
    : AComponent("SpriteRenderer", ComponentType::RENDERER), pTexture(nullptr)
{
    this->flipX = false;
    this->flipY = false;
    this->dAngle = 0.0;

    auto textures = TextureManager::getInstance()->getTexture(textureName);
    if (!textures.empty()) {
        pTexture = textures[0];
    }
    else {
        std::cerr << "[ERROR] : Texture not found: " << textureName << std::endl;
    }

    int texW = 0, texH = 0;
    if (pTexture) {
        float fw, fh;
        if (SDL_GetTextureSize(pTexture, &fw, &fh)) {
            texW = static_cast<int>(fw);
            texH = static_cast<int>(fh);
        }
    }


    //SDL_Point anchor = { texW / 2,texH / 2 };
    mDestRect.x = x;
    mDestRect.y = y;
    mDestRect.w = (w > 0) ? w : (float)texW;
    mDestRect.h = (h > 0) ? h : (float)texH;

    //mDestRect.x = anchor.x - (mDestRect.w / 2);
    //mDestRect.y = anchor.y - (mDestRect.h / 2);


}

void SpriteRenderer::draw(SDL_Renderer* pRenderer) {
    if (pTexture) {
        if (this->flipX && this->flipY) SDL_RenderTextureRotated(pRenderer, pTexture, NULL, &mDestRect, this->dAngle, NULL, SDL_FLIP_NONE); //replace with both flipped when available
        if (this->flipX) SDL_RenderTextureRotated(pRenderer, pTexture, NULL, &mDestRect, this->dAngle, NULL, SDL_FLIP_HORIZONTAL);
        else if (this->flipY) SDL_RenderTextureRotated(pRenderer, pTexture, NULL, &mDestRect, this->dAngle, NULL, SDL_FLIP_VERTICAL);
        else SDL_RenderTextureRotated(pRenderer, pTexture, NULL, &mDestRect, this->dAngle, NULL, SDL_FLIP_NONE);
    }

    // additional log
    else if (SDL_RenderTexture(pRenderer, pTexture, nullptr, &mDestRect) < 0) {
        SDL_Log("SDL_RenderTexture failed: %s", SDL_GetError());
    }
}

void SpriteRenderer::perform()
{

}

void SpriteRenderer::setTexture(SDL_Texture* pTexture)
{
    this->pTexture = pTexture;
}

void SpriteRenderer::setPosition(float x, float y) {
    mDestRect.x = x;
    mDestRect.y = y;
}

void SpriteRenderer::setSize(float w, float h) {
    mDestRect.w = w;
    mDestRect.h = h;
}


void SpriteRenderer::setFlipX(bool flipX)
{
    this->flipX = flipX;
}

void SpriteRenderer::setFlipY(bool flipY)
{
    this->flipY = flipY;
}

void SpriteRenderer::setAngle(double dAngle)
{
    this->dAngle = dAngle;
}

void SpriteRenderer::setScale(float fX, float fY)
{
    mDestRect.w *= fX;
    mDestRect.h *= fY;
}

SDL_Texture* SpriteRenderer::getTexture()
{
    return this->pTexture;
}

bool SpriteRenderer::getflipX()
{
    return this->flipX;
}

bool SpriteRenderer::getFlipY()
{
    return this->flipY;
}

double SpriteRenderer::getAngle()
{
    return this->dAngle;
}

