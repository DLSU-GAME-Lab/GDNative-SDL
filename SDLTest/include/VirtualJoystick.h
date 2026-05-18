#pragma once
#include "AComponent.h"
#include "Vector2D.h"
#include "Sprite.h"
#include "SpriteRenderer.h"

class PlayerInput;

class VirtualJoystick : public AComponent
{
public:
    VirtualJoystick(float radius);
    void perform() override;

private:
    Sprite* pThumb = nullptr;
    SpriteRenderer* pBase = nullptr;
    PlayerInput* playerInput = nullptr;
    Vector2D center;
    float radius;
    bool active = false;
    bool reset = false;

    void updateFromTouch(const Vector2D& pos);
    void resetTouch();

    void onAttach();
};
