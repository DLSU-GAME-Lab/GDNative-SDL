#include "RigidBody.h"
#include "AGameObject.h"
#include "PhysicsSystem.h"

RigidBody::RigidBody() : BoxCollider("RigidBody")
{
	this->fWeight = 1.0f;
	this->fDrag = 0.0f;
	this->bGravityEnabled = false;
	this->bGrounded = false;

	PhysicsSystem::getInstance()->addRigidBody(this);
}

void RigidBody::addForce(Vector2D force, bool instant)
{
	if (instant) this->force = force;
	else this->force += force * this->fDeltaTime;
}

void RigidBody::onCollisionEnter(ACollider* pCollider)
{
	BoxCollider::onCollisionEnter(pCollider);
	this->bGrounded = this->bCollideBottom;
}

void RigidBody::onCollisionContinue(ACollider* pCollider)
{
	BoxCollider::onCollisionContinue(pCollider);
	this->bGrounded = this->bCollideBottom;
}

void RigidBody::onCollisionExit(ACollider* pCollider)
{
	BoxCollider::onCollisionExit(pCollider);
	this->bGrounded = this->bCollideBottom;
}

void RigidBody::physicsUpdate()
{
	if (this->bGravityEnabled && !this->bGrounded)
		this->force.y -= F_GRAVITY * this->fWeight * this->fDeltaTime;

	Vector2D dragForce = (fDrag * 0.5f) * fWeight * -velocity;
	Vector2D totalForce = this->force + dragForce;

	//this->fTicks += this->fDeltaTime;
	//if (this->fTicks >= 2.0f)
	//{
	//	this->fTicks = 0.0f;
	//	std::cout << "Velocity: " << velocity << " Current Force: " << force << " Drag Force: " << dragForce << std::endl;
	//	std::cout << "Total Force: " << totalForce << std::endl;
	//}

	//this->force = totalForce;

	this->velocity += this->force * this->fDeltaTime;

	if (this->force.SqrMagnitude() < 0.01f)
		this->force = Vector2D::Zero();

	if (this->velocity.SqrMagnitude() < 0.01f)
		this->velocity = Vector2D::Zero();

	Vector2D pos = this->pOwner->getPos();
	this->pOwner->setPos(pos + this->velocity);
}

void RigidBody::physicsLateUpdate()
{
	if (this->intersection != Vector2D::Zero())
	{

		if (this->intersection.x != 0.0f)
		{
			this->force.x = 0.0f;
			this->velocity.x = 0.0f;
		}
		if (this->intersection.y != 0.0f)
		{
			this->force.y = 0.0f;
			this->velocity.y = 0.0f;
		}

		Vector2D pos = this->pOwner->getPos();
		this->pOwner->setPos(pos + this->intersection);
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

void RigidBody::setGravityEnabled(bool bGravityEnabled)
{
	this->bGravityEnabled = bGravityEnabled;
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

bool RigidBody::getGravityEnabled() const
{
	return this->bGravityEnabled;
}

bool RigidBody::getGrounded() const
{
	return this->bGrounded;
}
