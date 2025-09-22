#include "Player.h"
#include "TextureManager.h"
#include "RenderSystem.h"
#include "SpriteAnimator.h"
#include "PlayerController.h"

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
    SpriteRenderer* pSpriteRenderer = new SpriteRenderer("Player", this->fVecTranslate.x, this->fVecTranslate.y);
    RenderSystem::getInstance()->registerSpriteRenderer(pSpriteRenderer);
    auto vecSprite = TextureManager::getInstance()->getTexture("Player", 0, 15);

    SpriteAnimator* pSpriteAnimator = new SpriteAnimator(pSpriteRenderer, vecSprite, 12);
    PlayerInput* pPlayerInput = new PlayerInput();
    PlayerController* pPlayerController = new PlayerController(pPlayerInput, pSpriteRenderer);

    this->attachComponent(pSpriteRenderer);
    this->attachComponent(pSpriteAnimator);
    this->attachComponent(pPlayerInput);
    this->attachComponent(pPlayerController);

    pSpriteAnimator->setAnimationType(AnimationType::LOOP);
    pSpriteAnimator->play();
    pPlayerController->setMoveSpeed(300.0f);
    pPlayerController->setJumpForce(500.0f);
}
