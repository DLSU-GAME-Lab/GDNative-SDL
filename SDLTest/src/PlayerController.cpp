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
	this->bFalling = false;
	this->bJumping = false;
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

	if (this->pInput->getJumped())
	{
		this->pAnimator->play("jump");
		this->pRigidBody->addForce(Vector2D(0.0f, this->fJumpForce));
		this->bJumping = true;
		this->bFalling = false;
	}

}

void PlayerController::jump()
{

	//Gravity* pGrav = (Gravity*)this->pOwner->findComponentByName("Gravity");
	//if (pGrav != NULL && this->bJumping)
	//{
	//	this->fVelY -= pGrav->getGravValue() * this->fDeltaTime; // Gravity pulls down
	//	Vector2D pos = this->pOwner->getPos();
	//	pos.y += this->fVelY * this->fDeltaTime;
	//	this->pOwner->setPos(pos);

	//	// Detect apex
	//	if (this->fVelY <= 0.0f && !this->bFalling)
	//	{
	//		this->bFalling = true;
	//		this->bJumping = false;
	//		this->fVelY = 0;
	//		pGrav->setGrounded(false);
	//	}
	//}
}

void PlayerController::setMoveSpeed(float fMoveSpeed)
{
	this->fMoveSpeed = fMoveSpeed;
}

void PlayerController::setJumpForce(float fJumpForce)
{
	this->fJumpForce = fJumpForce;
}

void PlayerController::setVelY(float fVelY)
{
	this->fVelY = fVelY;
}


