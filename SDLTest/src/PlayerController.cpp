#include "PlayerController.h"
#include "AGameObject.h"
#include "AInteractable.h"
#include "AudioManager.h"

PlayerController::PlayerController(PlayerInput* pInput, SpriteRenderer* pSprite, SpriteAnimator* pAnimator, RigidBody* pRigidBody)
	: AComponent("PlayerController", ComponentType::SCRIPT)
{
	this->pInput = pInput;
	this->pSprite = pSprite;
	this->pAnimator = pAnimator;
	this->pRigidBody = pRigidBody;
	this->fMoveSpeed = 100.0f;
	this->fJumpForce = 100.0f;
	this->pQMark = NULL;

	this->pRigidBody->setListener(this);
}

PlayerController::~PlayerController()
{

}

void PlayerController::onAttach()
{
	this->pQMark = this->pOwner->findChildByName("Q_Mark");
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
			AudioManager::getInstance()->play(new AudioPlayer("Jump", AudioGroupTag::SFX));
			AudioManager::getInstance()->play(new AudioPlayer("Land", AudioGroupTag::SFX));

            // consume virtual jump so it only fires once
            this->pInput->setVirtualJump(false);
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
	if (AInteractable* pCollectable = dynamic_cast<AInteractable*>(pCollider))
	{
		std::cout << "collectable detected." << std::endl;
		this->pQMark->setEnabled(true);
	}
	else if (pCollider)
	{
		std::cout << pCollider->getOwner()->getName() << std::endl;
	}
}

void PlayerController::onCollisionContinue(ACollider * pCollider)
{
	if (AInteractable* pCollectable = dynamic_cast<AInteractable*>(pCollider))
	{
		if (this->pInput->getInteracted()) pCollectable->onInteract();
	}
}

void PlayerController::onCollisionExit(ACollider * pCollider)
{
	if (AInteractable* pCollectable = dynamic_cast<AInteractable*>(pCollider))
	{
		this->pQMark->setEnabled(false);
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
