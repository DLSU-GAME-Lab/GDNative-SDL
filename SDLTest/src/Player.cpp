#include "Player.h"
#include "TextureManager.h"
#include "SpriteAnimator.h"
#include "PlayerController.h"
#include "PhysicsSystem.h"
#include "Gravity.h"
#include "ColliderRenderer.h"
#include "CameraController.h"
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

    Animation* pIdle = new Animation("idle", vecIdle, 12, AnimationType::LOOP);
    Animation* pRun = new Animation("run", vecRun, 16, AnimationType::LOOP);
    Animation* pJump = new Animation("jump", vecJump, 8, AnimationType::ONCE, "idle");

    pSpriteAnimator->addAnimation(pIdle);
    pSpriteAnimator->addAnimation(pRun);
    pSpriteAnimator->addAnimation(pJump);
    pSpriteAnimator->play("idle");

    PlayerInput* pPlayerInput = new PlayerInput();
    this->attachComponent(pPlayerInput);

    RigidBody* pRB = new RigidBody();
    SDL_FRect size = SDL_FRect{ 0 ,0, 200, 460 };
    pRB->setSize(size);
    pRB->setListener(this);
    pRB->setWeight(10.0f);
    pRB->setDrag(1.0f);
    pRB->setGravityEnabled(true);
    PhysicsSystem::getInstance()->trackCollider(pRB);
    this->attachComponent(pRB);

    ColliderRenderer* pColRenderer = new ColliderRenderer(pRB);
    this->attachComponent(pColRenderer);

    PlayerController* pPlayerController = new PlayerController(pPlayerInput, pSpriteRenderer, pSpriteAnimator, pRB);
    pPlayerController->setMoveSpeed(2000.0f);
    pPlayerController->setJumpForce(5000.f);
    this->attachComponent(pPlayerController);

    CameraController* pCamComtroller = new CameraController();
    pCamComtroller->setOffset(Vector2D(0.0f, 200.0f));
    this->attachComponent(pCamComtroller);
}

void Player::onCollisionEnter(ACollider* pCollider)
{
    std::cout << "player collision" << std::endl;
}

void Player::onCollisionContinue(ACollider* pCollider)
{
}

void Player::onCollisionExit(ACollider* pCollider)
{
}
