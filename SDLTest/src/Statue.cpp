#include "Statue.h"
#include "SpriteRenderer.h"
#include "StatuePickup.h"
Statue::Statue(std::string strName): AGameObject(strName)
{
}

void Statue::initialize()
{
	this->fVecScale = Vector2D(0.25f);
	SpriteRenderer* pRenderer = new SpriteRenderer(this->strName);
	this->attachComponent(pRenderer);

	StatuePickup* pPickup = new StatuePickup();
	pPickup->setSize(Vector2D(220, 300));
	pPickup->setOffset(Vector2D(0.0f, 40.0f));
	this->attachComponent(pPickup);
}
