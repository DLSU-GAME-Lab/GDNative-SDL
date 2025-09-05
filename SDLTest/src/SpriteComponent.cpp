#include "SpriteComponent.h"
#include <SDL3_image/SDL_image.h>
#include <iostream>

// initialize the component and load the sprite
SpriteComponent::SpriteComponent(const std::string& path, SDL_Renderer* renderer, int x, int y)
    : Component("SpriteComponent", ComponentType::SPRITE),  
    mRenderer(renderer), mTexture(nullptr), mX(x), mY(y), mWidth(0), mHeight(0)
{
    load(path);
}

SpriteComponent::~SpriteComponent() {
    if (mTexture) {
        SDL_DestroyTexture(mTexture);
        mTexture = nullptr;
    }
}

// Loads the image from file and creates a texture
bool SpriteComponent::load(const std::string& path) {
    SDL_Surface* surface = IMG_Load(path.c_str());
    if (!surface) {
        std::cerr << "Failed to load image: " << path << " Error: " << SDL_GetError() << std::endl;
        return false;
    }

    // Convert surface to texture
    mTexture = SDL_CreateTextureFromSurface(mRenderer, surface);
    if (!mTexture) {
        std::cerr << "Failed to create texture: " << SDL_GetError() << std::endl;
        SDL_DestroySurface(surface);
        return false;
    }

    // Store sprite dimensions from the surface
    mWidth = surface->w;
    mHeight = surface->h;

    SDL_DestroySurface(surface);
    return true;
}

// Renders the sprite to the screen at its position
void SpriteComponent::perform() {
    if (!mTexture) return;

    SDL_FRect dest;
    dest.x = static_cast<float>(mX);
    dest.y = static_cast<float>(mY);
    dest.w = static_cast<float>(mWidth);
    dest.h = static_cast<float>(mHeight);

    SDL_RenderTexture(mRenderer, mTexture, nullptr, &dest);
}
