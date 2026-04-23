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

    SDL_Renderer* getSDLRenderer();
    void setSDLRenderer(SDL_Renderer* pRenderer);
    bool inCameraView(SDL_FRect spriteRect);
};

