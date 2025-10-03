#pragma once
#include "AComponent.h"
#include "PlayerInput.h"
#include "SpriteAnimator.h"

class PlayerController : public AComponent
{
private:
	PlayerInput* pInput;
	SpriteRenderer* pSprite;
	SpriteAnimator* pAnimator;
	float fMoveSpeed;
	float fJumpForce;

public:
	PlayerController(PlayerInput* pInput, SpriteRenderer* pSprite, SpriteAnimator* pAnimator);
	~PlayerController();

	void onAttach() override;
	void perform() override;

	void setMoveSpeed(float fMoveSpeed);
	void setJumpForce(float fJumpForce);
};

