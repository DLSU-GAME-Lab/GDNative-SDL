#include "PlayerController.h"
#include "AGameObject.h"

PlayerController::PlayerController(PlayerInput* pInput, SpriteRenderer* pSprite) : AComponent("Player Controller", ComponentType::SCRIPT)
{
	this->pInput = pInput;
	this->pSprite = pSprite;
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
		Vector2D pos = this->pOwner->getPos();
		pos.x += this->pInput->getMoveX() * this->fMoveSpeed * this->fDeltaTime;
		this->pOwner->setPos(pos);

		if (pos.x != 0.0f) this->pSprite->setFlipX(pos.x < 0.0f);
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
