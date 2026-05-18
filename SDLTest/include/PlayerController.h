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
	AGameObject* pQMark;
	float fMoveSpeed;
	float fJumpForce;

    float fMoveDirection = 0.0f;
    bool bMoving = false;
    bool bJumped = false;

public:
	PlayerController(PlayerInput* pInput, SpriteRenderer* pSprite, SpriteAnimator* pAnimator, RigidBody* pRigidBody);
	~PlayerController();

	void onAttach() override;
	void perform() override;

	void onCollisionEnter(ACollider* pCollider) override;
	void onCollisionContinue(ACollider* pCollider) override;
	void onCollisionExit(ACollider* pCollider) override;

	void Move(float direction);
	void Jump();

	void setMoveSpeed(float fMoveSpeed);
	void setJumpForce(float fJumpForce);
};

