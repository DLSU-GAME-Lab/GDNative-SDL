#include "Player.h"
#include "TextureManager.h"
#include "SpriteAnimator.h"
#include "PlayerController.h"
#include "PhysicsManager.h"
Player::Player(Vector2D fVecTranslate, Vector2D fVecScale, float fRot):AGameObject("Player")
{
    this->fVecTranslate = fVecTranslate;
    this->fVecScale = fVecScale;
    this->fRot = fRot;
}

Player::~Player()
{

}

void Player::initialize()
{
    //500, 630
    
    SpriteRenderer* pSpriteRenderer = new SpriteRenderer("player_idle", this->fVecTranslate.x, this->fVecTranslate.y);

    SpriteAnimator* pSpriteAnimator = new SpriteAnimator(pSpriteRenderer);
    PlayerInput* pPlayerInput = new PlayerInput();
    PlayerController* pPlayerController = new PlayerController(pPlayerInput, pSpriteRenderer, pSpriteAnimator);
    //Collider* pCollider = new Collider(this->strName + " Collider", true);
    //pCollider->setListener(this);
    //this->attachComponent((AComponent*)pCollider);
    //PhysicsManager::getInstance()->trackCollider(pCollider);

    this->attachComponent(pSpriteRenderer);
    this->attachComponent(pSpriteAnimator);
    this->attachComponent(pPlayerInput);
    this->attachComponent(pPlayerController);
    //std::cout << pCollider->getGlobalBounds().w << ", " << pCollider->getGlobalBounds().h << std::endl;
    
    auto vecIdle = TextureManager::getInstance()->getTexture("player_idle");
    auto vecRun = TextureManager::getInstance()->getTexture("player_run");
    pSpriteAnimator->addAnimationState("idle", vecIdle, 12);
    pSpriteAnimator->addAnimationState("run", vecRun, 16);
    pSpriteAnimator->setAnimationState("idle");
    pSpriteAnimator->setAnimationType(AnimationType::LOOP);
    pSpriteAnimator->play();
    pPlayerController->setMoveSpeed(300.0f);
    pPlayerController->setJumpForce(500.0f);
}

//void Player::onCollisionEnter(Collider* pCollider)
//{
//}
//
//void Player::onCollisionContinue(Collider* pCollider)
//{
//}
//
//void Player::onCollisionExit(Collider* pCollider)
//{
//}
