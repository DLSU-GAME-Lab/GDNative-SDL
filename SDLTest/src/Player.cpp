#include "Player.h"
#include "TextureManager.h"
#include "SpriteAnimator.h"
#include "PlayerController.h"
#include "PhysicsManager.h"
#include "Gravity.h"
#include "ColliderRenderer.h"
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

    PlayerInput* pPlayerInput = new PlayerInput();
    this->attachComponent(pPlayerInput);

    PlayerController* pPlayerController = new PlayerController(pPlayerInput, pSpriteRenderer, pSpriteAnimator);
    this->attachComponent(pPlayerController);

    Gravity* pGrav = new Gravity(150.f);
    this->attachComponent(pGrav);

    BoxCollider* pCollider = new BoxCollider(this->strName + " Collider", pSpriteRenderer->getRect());
    SDL_FRect COffset = SDL_FRect{ 0 ,0,-300,-250 };
    pCollider->setOffset(COffset);
    pCollider->setListener(this);
    PhysicsManager::getInstance()->trackCollider(pCollider);
    this->attachComponent(pCollider);


    ColliderRenderer* pColRenderer = new ColliderRenderer(pCollider->getGlobalBounds());
    this->attachComponent(pColRenderer);


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
    pPlayerController->setMoveSpeed(300.0f);
    pPlayerController->setJumpForce(250.f);
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
