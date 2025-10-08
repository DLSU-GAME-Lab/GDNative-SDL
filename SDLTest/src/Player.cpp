#include "Player.h"
#include "TextureManager.h"
#include "SpriteAnimator.h"
#include "PlayerController.h"
#include "PhysicsManager.h"
#include "Gravity.h"
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
    Gravity* pGrav = new Gravity(150.f);
    Collider* pCollider = new Collider(this->strName + " Collider", true);
   
    pCollider->setListener(this);
    PhysicsManager::getInstance()->trackCollider(pCollider);

    this->attachComponent(pSpriteRenderer);
    this->attachComponent(pSpriteAnimator);
    this->attachComponent(pPlayerInput);
    this->attachComponent(pPlayerController);
    this->attachComponent(pGrav);
    this->attachComponent(pCollider);
    SDL_FRect COffset = SDL_FRect{ 100 ,25,-220,-50 };
    pCollider->setOffset(COffset);
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

void Player::onCollisionEnter(Collider* pCollider)
{
}

void Player::onCollisionContinue(Collider* pCollider)
{
}

void Player::onCollisionExit(Collider* pCollider)
{
}
