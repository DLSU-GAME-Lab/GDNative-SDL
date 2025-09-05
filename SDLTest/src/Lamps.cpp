#include "Lamps.h"

Lamps::Lamps():AGameObject("Lamps")
{
}

Lamps::~Lamps()
{
}

void Lamps::initialize()
{

    SpriteRenderer* pSpriteRenderer = new SpriteRenderer("lobby_transition_lamps.png", 0, 0);
    SpriteRendererSystem::getInstance()->registerSpriteRenderer(pSpriteRenderer);
    this->attachComponent((AComponent*)pSpriteRenderer);
}
