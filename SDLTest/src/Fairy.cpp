#include "Fairy.h"
#include "SpriteRendererSystem.h"

Fairy::Fairy() : AGameObject("Fairy")
{

}

Fairy::~Fairy()
{

}

void Fairy::initialize()
{
    SpriteRenderer* pSpriteRenderer = new SpriteRenderer("fairy.png", 350, 500);
    SpriteRendererSystem::getInstance()->registerSpriteRenderer(pSpriteRenderer);
    this->attachComponent((AComponent*)pSpriteRenderer);
}
