#pragma once
#include "AComponent.h"
#include "Vector2D.h"
#include "CameraManager.h"

class ARenderer : public AComponent
{
protected:
    SDL_Renderer* pRenderer;

public:
    ARenderer(const std::string& strName);

    virtual void perform() = 0;
    virtual void drawWidget() = 0;

    void setSDLRenderer(SDL_Renderer* pRenderer);
    bool inCameraView(SDL_FRect spriteRect);
};

