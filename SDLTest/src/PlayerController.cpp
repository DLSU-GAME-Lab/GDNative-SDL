#include "PlayerController.h"
#include "AGameObject.h"
#include "AInteractable.h"
#include "AudioManager.h"
#include "InputManager.h"
#include "GameObjectManager.h"
#include "InteractButtonController.h"
// TODO: Update Player controller to use new input system
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

// TODO: Remove the pRigidBody calls and make this purely for animation updates
void PlayerController::perform()
{
	if (this->pInput == NULL || this->pSprite == NULL) return;

    if (bMoving)
    {
        this->bMoving = false;
        this->pRigidBody->setVelocity(Vector2D(fMoveDirection, 0.0f) * this->fMoveSpeed * this->fDeltaTime);
        this->pSprite->setFlipX(fMoveDirection < 0.0f);
        if (this->pRigidBody->getGrounded()) this->pAnimator->play("run");
    }
    else if (this->pRigidBody->getGrounded())
    {
        this->pAnimator->play("idle");
    }

    if (bJumped)
    {
        this->bJumped = false;
        if (this->pRigidBody->getGrounded())
        {
            this->pRigidBody->addForce(Vector2D(0.0f, this->fJumpForce), true);
            this->pAnimator->play("jump");

            AudioManager::getInstance()->play(new AudioPlayer("Jump", AudioGroupTag::SFX));
            AudioManager::getInstance()->play(new AudioPlayer("Land", AudioGroupTag::SFX));
        }
    }

    if (!this->pRigidBody->getGrounded() && this->pAnimator->getCurrentAnimation()->getName() != "idle")
    {
        this->pAnimator->play("fall");
    }
}

void PlayerController::onCollisionEnter(ACollider* pCollider)
{
	if (AInteractable* pCollectable = dynamic_cast<AInteractable*>(pCollider))
	{
		std::cout << "collectable detected." << std::endl;
		this->pQMark->setEnabled(true);
        this->pInput->setInteracted(false);
        AGameObject* pBtn = GameObjectManager::getInstance()->findObjectByName("InteractBtn");
        pBtn->setEnabled(true);
        InteractButtonController* pCtrl = (InteractButtonController*)pBtn->findComponentByName("InteractButtonController");
        if (pCtrl) pCtrl->setJustEnabled(true); 
	}
	else if (pCollider)
	{
		std::cout << pCollider->getOwner()->getName() << std::endl;
	}
}

void PlayerController::onCollisionContinue(ACollider* pCollider)
{
    if (AInteractable* pCollectable = dynamic_cast<AInteractable*>(pCollider))
    {
        if (this->pInput->getInteracted())
        {
            pCollectable->onInteract();
            this->pInput->setInteracted(false); 
        }
    }
}

void PlayerController::onCollisionExit(ACollider * pCollider)
{
	if (AInteractable* pCollectable = dynamic_cast<AInteractable*>(pCollider))
	{
		this->pQMark->setEnabled(false);
		GameObjectManager::getInstance()->findObjectByName("InteractBtn")->setEnabled(false);

	}
}

void PlayerController::Move(float direction)
{
	this->bMoving = true;
    this->fMoveDirection = direction;
}

void PlayerController::Jump()
{
	this->bJumped = true;
}

void PlayerController::setMoveSpeed(float fMoveSpeed)
{
	this->fMoveSpeed = fMoveSpeed;
}

void PlayerController::setJumpForce(float fJumpForce)
{
	this->fJumpForce = fJumpForce;
}
