#include "Librarian.h"
#include "TextureManager.h"
#include "SpriteRendererSystem.h"
#include "SpriteAnimator.h"

Librarian::Librarian() : AGameObject("Librarian")
{

}

Librarian::~Librarian()
{

}

void Librarian::initialize()
{
    SpriteRenderer* pSpriteRenderer = new SpriteRenderer("Librarian", 1000, 350);
    SpriteRendererSystem::getInstance()->registerSpriteRenderer(pSpriteRenderer);

    auto vecSprite = TextureManager::getInstance()->getTexture("Librarian", 0, 6);
    SpriteAnimator* pSpriteAnimator = new SpriteAnimator(pSpriteRenderer, vecSprite, 12);

    this->attachComponent((AComponent*)pSpriteRenderer);
    this->attachComponent((AComponent*)pSpriteAnimator);

    pSpriteRenderer->setFlipX(true);
    pSpriteAnimator->play();
}
