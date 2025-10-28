#pragma once
#include "AComponent.h"
#include "PlayerInput.h"
#include "SpriteAnimator.h"
#include "RigidBody.h"

class PlayerController : public AComponent
{
private:
	PlayerInput* pInput;
	SpriteRenderer* pSprite;
	SpriteAnimator* pAnimator;
	RigidBody* pRigidBody;
	float fMoveSpeed;
	float fJumpForce;
	float fVelY;
	bool bFalling;
	bool bJumping;

public:
	PlayerController(PlayerInput* pInput, SpriteRenderer* pSprite, SpriteAnimator* pAnimator, RigidBody* pRigidBody);
	~PlayerController();

	void perform() override;

	void jump();
	void setMoveSpeed(float fMoveSpeed);
	void setJumpForce(float fJumpForce);
	void setVelY(float fVelY);
};

