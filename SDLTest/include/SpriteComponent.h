#pragma once

#include "Component.h"
#include <SDL3/SDL.h>
#include <string>

// renders a sprite managed by TextureManager
class SpriteComponent : public Component {
public:
    // grabs a texture by name from TextureManager
    SpriteComponent(const std::string& name, SDL_Renderer* renderer, int x, int y);
    ~SpriteComponent() override;

    // called each frame to draw the sprite
    void perform() override;

private:
    SDL_Renderer* mRenderer;
    SDL_Texture* mTexture;

    int mX, mY;          // position
    int mWidth, mHeight; // dimensions
};
