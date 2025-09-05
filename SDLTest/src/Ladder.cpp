#include "Ladder.h"

Ladder::Ladder():AGameObject("Ladder")
{
}

Ladder::~Ladder()
{
}

void Ladder::initialize()
{
    SpriteRenderer* pSpriteRenderer = new SpriteRenderer("stepladder_revised.png", 600, 430, 700, 700);
    SpriteRendererSystem::getInstance()->registerSpriteRenderer(pSpriteRenderer);
    this->attachComponent((AComponent*)pSpriteRenderer);
}
