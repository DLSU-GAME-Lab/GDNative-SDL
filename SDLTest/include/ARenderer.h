#pragma once
#include "AComponent.h"
#include "Vector2D.h"
#include "CameraManager.h"

class ARenderer : public AComponent
{
protected:
    SDL_Renderer* pRenderer;
    SDL_Color mColor;

public:
    ARenderer(const std::string& strName);

    void setSDLRenderer(SDL_Renderer* pRenderer);
    bool inCameraView(SDL_FRect spriteRect);

    virtual void setColor(SDL_Color color);
    virtual SDL_Color getColor() const;
};

