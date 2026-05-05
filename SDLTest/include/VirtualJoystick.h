#pragma once
#include "AGeneralInput.h"
#include "Vector2D.h"
#include "Sprite.h"

class PlayerInput;

class VirtualJoystick : public AGeneralInput
{
public:
    VirtualJoystick(float radius);
    void perform() override;

private:
    Sprite* pThumb = nullptr;
    PlayerInput* playerInput = nullptr;
    Vector2D center;
    float radius;
    bool active = false;

    void updateFromTouch(const Vector2D& pos);

    void onAttach();
};
