#pragma once
#include "AGeneralInput.h"
#include "Vector2D.h"

class PlayerInput;

class VirtualJoystick : public AGeneralInput
{
public:
    VirtualJoystick(float radius);
    void perform() override;

private:
    PlayerInput* playerInput = nullptr;
    Vector2D center;
    float radius;
    float logicalX;
    float logicalY;
    bool active = false;

    void updateFromTouch(const Vector2D& pos);

    void onAttach();
};
