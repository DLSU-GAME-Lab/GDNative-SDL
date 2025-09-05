#pragma once

#include "AComponent.h"
#include <SDL3/SDL.h>
#include <string>

class SpriteRenderer : public AComponent {
private:
    SDL_Texture* pTexture;
    SDL_FRect mDestRect;

public:
    SpriteRenderer(const std::string& strTexName, float x = 0, float y = 0, float w = -1, float h = -1);

    void draw(SDL_Renderer* pRenderer);
    void perform() override;

    // setters
    void setPosition(float x, float y);
    void setSize(float w, float h);

    // getters
    SDL_FRect getRect() const { return mDestRect; }
};
