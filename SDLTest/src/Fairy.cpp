#include "Fairy.h"
#include "TextureManager.h"
#include "SpriteRendererSystem.h"
#include "SpriteAnimator.h"

Fairy::Fairy() : AGameObject("Fairy")
{

}

Fairy::~Fairy()
{

}

void Fairy::initialize()
{
    SpriteRenderer* pSpriteRenderer = new SpriteRenderer("Fairy", 350, 500);
    SpriteRendererSystem::getInstance()->registerSpriteRenderer(pSpriteRenderer);

    auto vecSprite = TextureManager::getInstance()->getTexture("Fairy", 0, 18);
    SpriteAnimator* pSpriteAnimator = new SpriteAnimator(pSpriteRenderer, vecSprite, 12);

    this->attachComponent((AComponent*)pSpriteRenderer);
    this->attachComponent((AComponent*)pSpriteAnimator);
    pSpriteAnimator->play();
}
