#include <iostream>
#include "SpriteRenderer.h"
#include "TextureManager.h"
#include "SpriteRendererSystem.h"

SpriteRenderer::SpriteRenderer(const std::string& textureName, float x, float y, float w, float h)
    : AComponent("SpriteRenderer", ComponentType::RENDERER), pTexture(nullptr)
{
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

    mDestRect.x = x;
    mDestRect.y = y;
    mDestRect.w = (w > 0) ? w : (float)texW;
    mDestRect.h = (h > 0) ? h : (float)texH;
}

void SpriteRenderer::draw(SDL_Renderer* pRenderer) {
    if (pTexture) {
        SDL_RenderTexture(pRenderer, pTexture, nullptr, &mDestRect);
        std::cout << "Drawing: " << this->strName << "\n";
    }
}

void SpriteRenderer::perform()
{

}

void SpriteRenderer::setPosition(float x, float y) {
    mDestRect.x = x;
    mDestRect.y = y;
}

void SpriteRenderer::setSize(float w, float h) {
    mDestRect.w = w;
    mDestRect.h = h;
}

