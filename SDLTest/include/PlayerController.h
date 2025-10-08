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
	float fVelY;
	bool bFalling;
	bool bJumping;

public:
	PlayerController(PlayerInput* pInput, SpriteRenderer* pSprite, SpriteAnimator* pAnimator);
	~PlayerController();

	void onAttach() override;
	void perform() override;

	void jump();
	void setMoveSpeed(float fMoveSpeed);
	void setJumpForce(float fJumpForce);
	void setVelY(float fVelY);
};

