#pragma once
#include "AComponent.h"
#include "PlayerInput.h"
#include "SpriteRenderer.h"

class JumpButtonController : public AComponent
{
public:
    JumpButtonController();

    void perform() override;

private:
    PlayerInput* playerInput = nullptr;
    SpriteRenderer* pRenderer = nullptr;
    bool bJumped = false;

    void onAttach();
};