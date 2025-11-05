#include "PlayerController.h"
#include "AGameObject.h"
#include "Gravity.h"
PlayerController::PlayerController(PlayerInput* pInput, SpriteRenderer* pSprite, SpriteAnimator* pAnimator, RigidBody* pRigidBody)
	: AComponent("PlayerController", ComponentType::SCRIPT)
{
	this->pInput = pInput;
	this->pSprite = pSprite;
	this->pAnimator = pAnimator;
	this->pRigidBody = pRigidBody;
	this->fMoveSpeed = 100.0f;
	this->fJumpForce = 100.0f;
}

PlayerController::~PlayerController()
{

}

void PlayerController::perform()
{
	if (this->pInput == NULL || this->pSprite == NULL) return;

	if (this->pInput->getMovement() != Vector2D::Zero())
	{
		if (this->pAnimator->getCurrentAnimation()->getName() != "jump")
			this->pAnimator->play("run");

		this->pRigidBody->addForce(this->pInput->getMovement() * this->fMoveSpeed);
		this->pSprite->setFlipX(this->pInput->getMovement().x < 0.0f);
	}
	else if(this->pAnimator->getCurrentAnimation()->getName() != "jump")
		this->pAnimator->setAnimationState("idle");

	if (this->pInput->getJumped() && this->pRigidBody->getGrounded())
	{
		this->pAnimator->play("jump");
		this->pRigidBody->addForce(Vector2D(0.0f, this->fJumpForce), true);
	}

}

void PlayerController::setMoveSpeed(float fMoveSpeed)
{
	this->fMoveSpeed = fMoveSpeed;
}

void PlayerController::setJumpForce(float fJumpForce)
{
	this->fJumpForce = fJumpForce;
}
