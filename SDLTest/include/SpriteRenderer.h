#pragma once

#include "AComponent.h"
#include "Vector2D.h"
#include "Camera.h"
#include <SDL3/SDL.h>
#include <string>

class SpriteRenderer : public AComponent
{
private:
    SDL_Texture* pTexture;
    SDL_FRect mDestRect;
    bool flipX, flipY;
    double dAngle;
    float fTexW, fTexH;
    std::string m_textureKey;
    Vector2D pivot;

public:
    SpriteRenderer(const std::string& strTexName, float x = 0, float y = 0, float w = -1, float h = -1);
    void initialize(); // (when all resources are guaranteed to be loaded).

    ~SpriteRenderer();

    void draw(SDL_Renderer* pRenderer, Camera* pCamera);
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

    // getters
    SDL_Texture* getTexture();
    SDL_FRect getRect() const { return mDestRect; }
    bool getflipX();
    bool getFlipY();
    double getAngle();
    Vector2D getPivot();

};
