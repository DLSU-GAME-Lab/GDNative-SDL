#include "Player.h"
#include "TextureManager.h"
#include "RenderSystem.h"
#include "SpriteAnimator.h"

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

    this->attachComponent((AComponent*)pSpriteRenderer);
    this->attachComponent((AComponent*)pSpriteAnimator);
    pSpriteAnimator->setAnimationType(AnimationType::LOOP);
    pSpriteAnimator->play();
}
