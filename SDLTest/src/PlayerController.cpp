#include "PlayerController.h"
#include "AGameObject.h"

PlayerController::PlayerController(PlayerInput* pInput, SpriteRenderer* pSprite, SpriteAnimator* pAnimator)
	: AComponent("PlayerController", ComponentType::SCRIPT)
{
	this->pInput = pInput;
	this->pSprite = pSprite;
	this->pAnimator = pAnimator;
	this->fMoveSpeed = 0.0f;
	this->fJumpForce = 0.0f;
}

PlayerController::~PlayerController()
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
}

void PlayerController::setMoveSpeed(float fMoveSpeed)
{
	this->fMoveSpeed = fMoveSpeed;
}

void PlayerController::setJumpForce(float fJumpForce)
{
	this->fJumpForce = fJumpForce;
}
