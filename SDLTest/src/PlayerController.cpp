#include "PlayerController.h"
#include "AGameObject.h"
#include "ACollectable.h"

PlayerController::PlayerController(PlayerInput* pInput, SpriteRenderer* pSprite, SpriteAnimator* pAnimator, RigidBody* pRigidBody)
	: AComponent("PlayerController", ComponentType::SCRIPT)
{
	this->pInput = pInput;
	this->pSprite = pSprite;
	this->pAnimator = pAnimator;
	this->pRigidBody = pRigidBody;
	this->fMoveSpeed = 100.0f;
	this->fJumpForce = 100.0f;

	this->pRigidBody->setListener(this);
}

PlayerController::~PlayerController()
{

}

void PlayerController::perform()
{
	if (this->pInput == NULL || this->pSprite == NULL) return;

	this->pRigidBody->setVelocity(this->pInput->getMovement() * this->fMoveSpeed * this->fDeltaTime);

	if (this->pInput->getMovement() != Vector2D::Zero())
	{
		this->pSprite->setFlipX(this->pInput->getMovement().x < 0.0f);
	}
	
	std::string animName = this->pAnimator->getCurrentAnimation()->getName();
	if (this->pRigidBody->getGrounded())
	{
		if (this->pInput->getJumped())
		{
			this->pAnimator->play("jump");
			this->pRigidBody->addForce(Vector2D(0.0f, this->fJumpForce), true);
		}

		if (this->pInput->getMovement() != Vector2D::Zero())
		{
			this->pAnimator->play("run");
		}
		else this->pAnimator->setAnimationState("idle");
	}
	else this->pAnimator->play("fall");
}

void PlayerController::onCollisionEnter(ACollider* pCollider)
{
	if (ACollectable* pCollectable = dynamic_cast<ACollectable*>(pCollider))
	{
		std::cout << "collectable detected." << std::endl;
	}
}

void PlayerController::onCollisionContinue(ACollider * pCollider)
{
	if (ACollectable* pCollectable = dynamic_cast<ACollectable*>(pCollider))
	{
		if (this->pInput->getInteracted()) pCollectable->onCollect();
	}
}

void PlayerController::onCollisionExit(ACollider * pCollider)
{

}

void PlayerController::setMoveSpeed(float fMoveSpeed)
{
	this->fMoveSpeed = fMoveSpeed;
}

void PlayerController::setJumpForce(float fJumpForce)
{
	this->fJumpForce = fJumpForce;
}
