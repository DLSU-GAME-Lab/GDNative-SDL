#include "Chair.h"

Chair::Chair(bool bFlipX): AGameObject("Chair"),bFlipX(bFlipX)
{
}

Chair::~Chair()
{
}

void Chair::initialize()
{
	SpriteRenderer* pSpriteRenderer = new SpriteRenderer("monoblock_revised.png", 0, 630);
	if (bFlipX)
	{
		pSpriteRenderer->setFlipX(true);
	}
	SpriteRendererSystem::getInstance()->registerSpriteRenderer(pSpriteRenderer);
	this->attachComponent((AComponent*)pSpriteRenderer);
}
