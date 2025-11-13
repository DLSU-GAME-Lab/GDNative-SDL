#include "Gate.h"
#include "SpriteRenderer.h"
#include "InteractabeGate.h"

Gate::Gate(std::string strName) : AGameObject(strName)
{

}

void Gate::initialize()
{
	SpriteRenderer* pRenderer = new SpriteRenderer("Gate");
	this->attachComponent(pRenderer);

	BoxCollider* pCollider = new BoxCollider("BoxCollider");
	pCollider->setSize(Vector2D(100, 400));
	this->attachComponent(pCollider);

	InteractabeGate* pInteractable = new InteractabeGate();
	pInteractable->setSize(Vector2D(200, 400));
	this->attachComponent(pInteractable);
}
