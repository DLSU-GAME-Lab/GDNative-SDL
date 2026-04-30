#include "Gate.h"
#include "SpriteRenderer.h"
#include "InteractableGate.h"
#include "TweenAnimator.h"
#include "tweeny.h"

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

	InteractableGate* pInteractable = new InteractableGate();
	pInteractable->setSize(Vector2D(200, 400));
	this->attachComponent(pInteractable);

	TweenAnimator* pAnimator = new TweenAnimator();
	this->attachComponent(pAnimator);


	Vector2D startPos = this->getPos();
	Vector2D endPos = Vector2D(this->getPos().x, this->getPos().y + 500);

	tweeny::tween<float, float> tweenPos = tweeny::tween<float, float>::from(startPos.x, startPos.y)
		.to(endPos.x, endPos.y).during(1000);

	tweenPos = tweenPos.via(tweeny::easing::linear);
	pAnimator->setTweenPos(tweenPos);
}

void Gate::gateOpen()
{
	InteractableGate* pInteractable = (InteractableGate*)this->findComponentByName("InteractableGate");
	pInteractable->setEnabled(false);
;

	TweenAnimator* pAnimator = (TweenAnimator*)this->findComponentByName("TweenAnimator");
	pAnimator->play();
}
