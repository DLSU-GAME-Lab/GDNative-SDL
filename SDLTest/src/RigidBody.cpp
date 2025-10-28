#include "RigidBody.h"
#include "AGameObject.h"

RigidBody::RigidBody() : BoxCollider("RigidBody", {0, 0, 0, 0})
{
	this->fWeight = 1.0f;
	this->fDrag = 0.05f;
	this->bGravityEnabled = false;
	this->bGrounded = false;
}

void RigidBody::addForce(Vector2D force)
{
	this->velocity += (force / this->fWeight) * this->fDeltaTime;
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

	if (this->bGravityEnabled && !this->bGrounded)
		this->velocity.y -= F_GRAVITY * this->fDeltaTime;

	if ((this->bCollideLeft && this->velocity.x < 0.0f) ||
		this->bCollideRight && this->velocity.x > 0.0f)
		this->velocity.x = 0.0f;

	if ((this->bCollideBottom && this->velocity.y < 0.0f) ||
		this->bCollideTop && this->velocity.y > 0.0f)
		this->velocity.y = 0.0f;

	this->pOwner->setPos(pos - this->intersection + this->velocity);
	pos = this->pOwner->getPos();
	this->rectShape.setPosition(pos.x, pos.y);

	if (this->velocity != Vector2D::Zero())
	{
		Vector2D dragForce = (fDrag / 2) * velocity * velocity;
		if (velocity.x > 0.0f) velocity.x = std::max(0.0f, velocity.x - dragForce.x);
		else if (velocity.x < 0.0f) velocity.x = std::min(0.0f, velocity.x + dragForce.x);
		if (velocity.y > 0.0f) velocity.y = std::max(0.0f, velocity.y - dragForce.y);
		else if (velocity.y < 0.0f) velocity.y = std::max(0.0f, velocity.y + dragForce.y);
	}
}

void RigidBody::setWeight(float fWeight)
{
	this->fWeight = fWeight;
}

void RigidBody::setDrag(float fDrag)
{
	this->fDrag = fDrag;
}

void RigidBody::setVelocity(Vector2D velocity)
{
	this->velocity = velocity;
}

float RigidBody::getWeight() const
{
	return this->fWeight;
}

float RigidBody::getDrag() const
{
	return this->fDrag;
}

Vector2D RigidBody::getVelocity() const
{
	return this->velocity;
}
