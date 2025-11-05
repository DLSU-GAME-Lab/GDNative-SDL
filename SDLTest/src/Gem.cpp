#include "Gem.h"
#include "SpriteRenderer.h"
#include "BoxCollider.h"
#include "ColliderRenderer.h"
#include "CollectableGem.h"

Gem::Gem(std::string strName) : AGameObject(strName)
{

}

void Gem::initialize()
{
	this->fVecScale = Vector2D(0.25f);
	SpriteRenderer* pRenderer = new SpriteRenderer(this->strName);
	this->attachComponent(pRenderer);

	BoxCollider* pCollider = new BoxCollider("BoxCollider");
	pCollider->setSize(Vector2D(220, 300));
	pCollider->setOffset(Vector2D(0.0f, 40.0f));
	this->attachComponent(pCollider);

	ColliderRenderer* pColliderRenderer = new ColliderRenderer(pCollider);
	this->attachComponent(pColliderRenderer);

	CollectableGem* pCollectable = new CollectableGem();
	this->attachComponent(pCollectable);
}
