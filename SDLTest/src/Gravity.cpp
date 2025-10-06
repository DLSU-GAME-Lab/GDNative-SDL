#include "Gravity.h"
#include "AGameObject.h"
Gravity::Gravity(float fGravValue) :AComponent("Gravity", ComponentType::SCRIPT)
{
	this->fGravValue = fGravValue;
	this->bGrounded = true;
}

Gravity::~Gravity()
{
}

void Gravity::perform()
{
	if (!bGrounded)
	{
		AGameObject* pOwner = this->getOwner();
		float fValue =pOwner->getPos().y - this->fGravValue * this->fDeltaTime;
		pOwner->setPos(Vector2D(pOwner->getPos().x, fValue));
		//std::cout << pOwner->getPos().x << ", " << pOwner->getPos().y << std::endl;
		//std::cout << "Not Grounded" << std::endl;
	}
}

void Gravity::setGravValue(float fGravValue)
{
	this->fGravValue = fGravValue;
}

float Gravity::getGravValue()
{
	return this->fGravValue;
}

void Gravity::setGrounded(bool bGrounded)
{
	this->bGrounded = bGrounded;
}

bool Gravity::isGrounded()
{
	return this->bGrounded;
}
