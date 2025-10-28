#include "RigidBody.h"
#include "AGameObject.h"

RigidBody::RigidBody() : BoxCollider("RigidBody", {0, 0, 0, 0})
{
	this->fWeight = 1.0f;
	this->bGrounded = false;
}

void RigidBody::move(Vector2D force)
{
	this->velocity += force * this->fDeltaTime;
	if (!this->bGrounded) this->velocity.y -= F_GRAVITY * this->fDeltaTime;
}

void RigidBody::onCollisionEnter(ACollider* pCollider)
{
	BoxCollider::onCollisionEnter(pCollider);
}

void RigidBody::onCollisionContinue(ACollider* pCollider)
{
	BoxCollider::onCollisionContinue(pCollider);
}

void RigidBody::onCollisionExit(ACollider* pCollider)
{
	BoxCollider::onCollisionExit(pCollider);
}

void RigidBody::onUpdate()
{
	Vector2D pos = this->pOwner->getPos();

	if ((this->bCollideLeft && this->velocity.x < 0.0f) ||
		this->bCollideRight && this->velocity.x > 0.0f)
		this->velocity.x = 0.0f;

	if ((this->bCollideBottom && this->velocity.y < 0.0f) ||
		this->bCollideTop && this->velocity.y > 0.0f)
		this->velocity.y = 0.0f;

	this->pOwner->setPos(pos + this->velocity);
	pos = this->pOwner->getPos();
	this->rectShape.setPosition(pos.x, pos.y);
}

void RigidBody::setWeight(float fWeight)
{
	this->fWeight = fWeight;
}

void RigidBody::setVelocity(Vector2D velocity)
{
	this->velocity = velocity;
}

float RigidBody::getWeight() const
{
	return this->fWeight;
}

Vector2D RigidBody::getVelocity() const
{
	return this->velocity;
}
