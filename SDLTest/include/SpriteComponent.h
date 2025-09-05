#pragma once

#include "Component.h"
#include <SDL3/SDL.h>
#include <string>

class SpriteComponent : public Component {
public:
    SpriteComponent(const std::string& path, SDL_Renderer* renderer, int x, int y);
    ~SpriteComponent() override;

    bool load(const std::string& path);

    // override base Component interface
    void perform() override;

private:
    SDL_Renderer* mRenderer;
    SDL_Texture* mTexture;

    int mX, mY;          // Position
    int mWidth, mHeight; // Size
};
