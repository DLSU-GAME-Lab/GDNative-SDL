#include <iostream>
#include "SpriteRenderer.h"
#include "TextureManager.h"
#include "SpriteRendererSystem.h"
#include <iostream>

SpriteRenderer::SpriteRenderer(const std::string& textureName, SDL_Renderer* renderer,
    float x, float y, float w, float h)
    : AComponent("SpriteRenderer", ComponentType::SPRITE), mTexture(nullptr)
{
    auto textures = TextureManager::getInstance()->getTexture(textureName);
    if (!textures.empty()) {
        mTexture = textures[0];
    }
    else {
        std::cerr << "[ERROR] : Texture not found: " << textureName << std::endl;
    }

    int texW = 0, texH = 0;
    if (mTexture) {
        float fw, fh;
        if (SDL_GetTextureSize(mTexture, &fw, &fh)) {
            texW = static_cast<int>(fw);
            texH = static_cast<int>(fh);
        }
    }

    mDestRect.x = x;
    mDestRect.y = y;
    mDestRect.w = (w > 0) ? w : (float)texW;
    mDestRect.h = (h > 0) ? h : (float)texH;
}

void SpriteRenderer::render(SDL_Renderer* renderer) {
    if (mTexture) {
        SDL_RenderTexture(renderer, mTexture, nullptr, &mDestRect);
    }
}

void SpriteRenderer::setPosition(float x, float y) {
    mDestRect.x = x;
    mDestRect.y = y;
}

void SpriteRenderer::setSize(float w, float h) {
    mDestRect.w = w;
    mDestRect.h = h;
}

