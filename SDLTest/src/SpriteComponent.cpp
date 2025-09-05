#include "SpriteComponent.h"
#include "TextureManager.h"
#include <iostream>

// fetches texture from TextureManager and queries its size
SpriteComponent::SpriteComponent(const std::string& name, SDL_Renderer* renderer, int x, int y)
    : Component("SpriteComponent", ComponentType::SPRITE),
    mRenderer(renderer), mTexture(nullptr), mX(x), mY(y), mWidth(0), mHeight(0)
{
    // Fetch texture by name (must be loaded earlier in TextureManager)
    mTexture = TextureManager::getInstance()->get(name);

    if (mTexture) {
        // Query width and height of the texture
        float w, h;
        if (SDL_GetTextureSize(mTexture, &w, &h)) {
            mWidth = static_cast<int>(w);
            mHeight = static_cast<int>(h);
        }
        else {
            SDL_Log("Failed to query texture: %s", SDL_GetError());
        }
    }
}

SpriteComponent::~SpriteComponent() {
    // do not destroy the texture here — TextureManager owns it
    mTexture = nullptr;
}

// Renders the sprite at (x, y)
void SpriteComponent::perform() {
    if (!mTexture) return;

    SDL_FRect dest{ (float)mX, (float)mY, (float)mWidth, (float)mHeight };
    SDL_RenderTexture(mRenderer, mTexture, nullptr, &dest);
}
