#include "Librarian.h"
#include "SpriteRendererSystem.h"

Librarian::Librarian() : AGameObject("Librarian")
{

}

Librarian::~Librarian()
{

}

void Librarian::initialize()
{
    SpriteRenderer* pSpriteRenderer = new SpriteRenderer("librarian.png", 1000, 500);
    SpriteRendererSystem::getInstance()->registerSpriteRenderer(pSpriteRenderer);
    this->attachComponent((AComponent*)pSpriteRenderer);
}
