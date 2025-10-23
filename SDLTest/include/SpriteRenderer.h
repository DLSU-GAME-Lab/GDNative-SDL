#pragma once

#include "ARenderer.h"
#include "Vector2D.h"
#include "CameraManager.h"
#include <SDL3/SDL.h>
#include <string>

class SpriteRenderer : public ARenderer
{
private:
    SDL_Texture* pTexture;
    SDL_Color mColor;
    SDL_FRect mDestRect;
    bool flipX, flipY;
    double dAngle;
    std::string m_textureKey;
    Vector2D texSize;
    Vector2D pivot;

public:
    SpriteRenderer(const std::string& strTexName, SDL_Color color = { 255, 255, 255, 255 });
    void initialize(); // (when all resources are guaranteed to be loaded).

    ~SpriteRenderer();

    void perform() override;

    // setters
    void setTexture(SDL_Texture* pTexture);
    void setPosition(float x, float y);

    //remove later if set scale is what this should do
    void setSize(float w, float h);
    void setFlipX(bool flipX);
    void setFlipY(bool flipY);
    void setAngle(double dAngle);
    void setPivot(Vector2D pivot);
    void setColor(SDL_Color color);

    // getters
    SDL_Texture* getTexture();
    SDL_Color getColor() const;
    SDL_FRect getRect() const;
    bool getflipX();
    bool getFlipY();
    double getAngle();
    Vector2D getPivot();

};
