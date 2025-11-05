#pragma once
#include "AComponent.h"
#include "PlayerInput.h"
#include "SpriteAnimator.h"
#include "RigidBody.h"

class PlayerController : public AComponent, public ICollisionListener
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

	void onCollisionEnter(ACollider* pCollider) override;
	void onCollisionContinue(ACollider* pCollider) override;
	void onCollisionExit(ACollider* pCollider) override;

	void setMoveSpeed(float fMoveSpeed);
	void setJumpForce(float fJumpForce);
};

