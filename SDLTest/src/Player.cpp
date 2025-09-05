#include "Player.h"
#include "SpriteRendererSystem.h"

Player::Player() : AGameObject("Player")
{

}

Player::~Player()
{

}

void Player::initialize()
{
    SpriteRenderer* pSpriteRenderer = new SpriteRenderer("frame1.png", 500, 500);
    SpriteRendererSystem::getInstance()->registerSpriteRenderer(pSpriteRenderer);
    this->attachComponent((AComponent*)pSpriteRenderer);
}
