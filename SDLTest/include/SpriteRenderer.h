#pragma once

#include "AComponent.h"
#include <SDL3/SDL.h>
#include <string>

// renders a sprite managed by TextureManager
class SpriteRenderer : public AComponent {
public:
    // grabs a texture by name from TextureManager
    SpriteRenderer(const std::string& name, SDL_Renderer* renderer, int x, int y);
    ~SpriteRenderer() override;

    // called each frame to draw the sprite
    void draw();

    void perform() override;

private:
    SDL_Renderer* mRenderer;
    SDL_Texture* mTexture;

    int mX, mY;          // position
    int mWidth, mHeight; // dimensions
};
