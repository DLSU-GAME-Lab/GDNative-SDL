#include "PlayerController.h"
#include "AGameObject.h"
#include "Gravity.h"
PlayerController::PlayerController(PlayerInput* pInput, SpriteRenderer* pSprite, SpriteAnimator* pAnimator)
	: AComponent("PlayerController", ComponentType::SCRIPT)
{
	this->pInput = pInput;
	this->pSprite = pSprite;
	this->pAnimator = pAnimator;
	this->fMoveSpeed = 0.0f;
	this->fJumpForce = 0.0f;
	this->bFalling = false;
}

PlayerController::~PlayerController()
{

}

void PlayerController::onAttach()
{

}

void PlayerController::perform()
{
	if (this->pInput == NULL || this->pSprite == NULL) return;

	if (this->pInput->getMoveX() != 0.0f)
	{
		this->pAnimator->setAnimationState("run");
		Vector2D pos = this->pOwner->getPos();
		pos.x += this->pInput->getMoveX() * this->fMoveSpeed * this->fDeltaTime;
		this->pOwner->setPos(pos);
		this->pSprite->setFlipX(this->pInput->getMoveX() < 0.0f);
	}
	else this->pAnimator->setAnimationState("idle");

	if (this->pInput->getJumped()&& !this->bJumping)
	{
		this->pAnimator->setAnimationState("jump");
		this->fVelY = this->fJumpForce;
		this->bJumping = true;
		this->bFalling = false;
	}
	this->jump();
		
}

void PlayerController::jump()
{
	Gravity* pGrav = (Gravity*)this->pOwner->findComponentByName("Gravity");
	if (pGrav != NULL && this->bJumping)
	{
		this->fVelY -= pGrav->getGravValue() * this->fDeltaTime; // Gravity pulls down
		Vector2D pos = this->pOwner->getPos();
		pos.y += this->fVelY * this->fDeltaTime;
		this->pOwner->setPos(pos);

		// Detect apex
		if (this->fVelY <= 0.0f && !this->bFalling)
		{
			this->bFalling = true;
			this->bJumping = false;
			this->fVelY = 0;
			pGrav->setGrounded(false);
		}
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

void PlayerController::setVelY(float fVelY)
{
	this->fVelY = fVelY;
}


