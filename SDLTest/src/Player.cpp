#include "Player.h"
#include "TextureManager.h"
#include "SpriteAnimator.h"
#include "PlayerController.h"
#include "PhysicsSystem.h"
#include "CameraController.h"
#include "Sprite.h"

Player::Player(Vector2D fVecTranslate, Vector2D fVecScale, float fRot):AGameObject("Player")
{
    this->fVecTranslate = fVecTranslate;
    this->fVecScale = fVecScale;
    this->fRot = fRot;
}

void Player::initialize()
{
    
    SpriteRenderer* pSpriteRenderer = new SpriteRenderer("player_idle");
    this->attachComponent(pSpriteRenderer);

    SpriteAnimator* pSpriteAnimator = new SpriteAnimator(pSpriteRenderer);
    this->attachComponent(pSpriteAnimator);

    auto vecIdle = TextureManager::getInstance()->getTexture("player_idle");
    auto vecRun = TextureManager::getInstance()->getTexture("player_run");
    auto vecJump = TextureManager::getInstance()->getTexture("player_jump");
    auto vecFall = TextureManager::getInstance()->getTexture("player_fall");

    Animation* pIdle = new Animation("idle", vecIdle, 12, AnimationType::LOOP);
    Animation* pRun = new Animation("run", vecRun, 16, AnimationType::LOOP);
    Animation* pJump = new Animation("jump", vecJump, 8, AnimationType::ONCE, OnAnimFinished::NEXT, "fall");
    Animation* pFall = new Animation("fall", vecFall, 8, AnimationType::LOOP);

    pSpriteAnimator->addAnimation(pIdle);
    pSpriteAnimator->addAnimation(pRun);
    pSpriteAnimator->addAnimation(pJump);
    pSpriteAnimator->addAnimation(pFall);
    pSpriteAnimator->play("idle");

    PlayerInput* pPlayerInput = new PlayerInput();
    this->attachComponent(pPlayerInput);

    RigidBody* pRB = new RigidBody();
    pRB->setSize(Vector2D(200, 460));
    pRB->setWeight(80.0f);
    pRB->setDrag(0.0f);
    pRB->setGravityEnabled(true);
    this->attachComponent(pRB);

    Sprite* pQMark = new Sprite("Q_Mark", "Q_Mark", Vector2D(0.0f, 0.0f), Vector2D(0.2f, 0.2f), -30.0f);
    this->attachChild(pQMark);
    pQMark->setLocalPos(Vector2D(100.0f, 200.0f));
    pQMark->setEnabled(false);

    PlayerController* pPlayerController = new PlayerController(pPlayerInput, pSpriteRenderer, pSpriteAnimator, pRB);
    pPlayerController->setMoveSpeed(300.0f);
    pPlayerController->setJumpForce(150.f);
    this->attachComponent(pPlayerController);

    CameraController* pCamComtroller = new CameraController();
    pCamComtroller->setFollowDelay(500.0f);
    pCamComtroller->setOffset(Vector2D(0.0f, 200.0f));
    pCamComtroller->setLookAhead(Vector2D(500.0f, 100.0f));
    this->attachComponent(pCamComtroller);
}

void Player::onCollisionEnter(ACollider* pCollider)
{
}

void Player::onCollisionContinue(ACollider* pCollider)
{
}

void Player::onCollisionExit(ACollider* pCollider)
{
}
