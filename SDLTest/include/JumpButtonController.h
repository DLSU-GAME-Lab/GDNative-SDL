#pragma once
#include "AComponent.h"
#include "PlayerInput.h"

class JumpButtonController : public AComponent
{
public:
    JumpButtonController();

    void perform() override;

private:
    PlayerInput* playerInput = nullptr;

    void onAttach();
};