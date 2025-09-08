#include "Player.h"
#include "TextureManager.h"
#include "SpriteRendererSystem.h"
#include "SpriteAnimator.h"

Player::Player(float fPosX, float fPosY, float fScaleX, float fScaleY, float fRot):AGameObject("Player")
{
    this->fPosX = fPosX;
    this->fPosY = fPosY;
    this->fScaleX = fScaleX;
    this->fScaleY = fScaleY;
    this->fRot = fRot;
}

Player::~Player()
{

}

void Player::initialize()
{
    //500, 630
    SpriteRenderer* pSpriteRenderer = new SpriteRenderer("Player", this->fPosX, this->fPosY);
    pSpriteRenderer->setScale(this->fScaleX, this->fScaleY);
    SpriteRendererSystem::getInstance()->registerSpriteRenderer(pSpriteRenderer);

    auto vecSprite = TextureManager::getInstance()->getTexture("Player", 0, 15);
    SpriteAnimator* pSpriteAnimator = new SpriteAnimator(pSpriteRenderer, vecSprite, 12);

    this->attachComponent((AComponent*)pSpriteRenderer);
    this->attachComponent((AComponent*)pSpriteAnimator);
    pSpriteAnimator->play();
}
