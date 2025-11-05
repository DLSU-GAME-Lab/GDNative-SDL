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

public:
	PlayerController(PlayerInput* pInput, SpriteRenderer* pSprite, SpriteAnimator* pAnimator, RigidBody* pRigidBody);
	~PlayerController();

	void perform() override;

	void setMoveSpeed(float fMoveSpeed);
	void setJumpForce(float fJumpForce);
};

