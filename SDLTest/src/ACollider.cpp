#include "ACollider.h"
#include "PhysicsSystem.h"

ACollider::ACollider(std::string strName) : AComponent(strName,ComponentType::SCRIPT)
{
	this->bCleanUp = false;
	this->bIsTrigger = false;
	this->pListener = NULL;

	PhysicsSystem::getInstance()->trackCollider(this);
}

int ACollider::findCollider(ACollider* pCollider)
{
	int nIndex = -1;

	for (int i = 0; i < this->vecCollided.size() && nIndex == -1; i++) {
		if (pCollider == this->vecCollided[i])
			nIndex = i;
	}

	return nIndex;
}



void ACollider::cleanCollisions()
{
	this->vecCollided.clear();
}

void ACollider::onCollisionEnter(ACollider* pCollider)
{
	if (this->pListener != NULL) {
		this->pListener->onCollisionEnter(pCollider);
	}
}

void ACollider::onCollisionContinue(ACollider* pCollider)
{
	if (this->pListener != NULL) {
		this->pListener->onCollisionContinue(pCollider);
	}
}

void ACollider::onCollisionExit(ACollider* pCollider)
{
	if (this->pListener != NULL) {
		this->pListener->onCollisionExit(pCollider);
		this->intersection = Vector2D::Zero();
	}
}

void ACollider::setListener(ICollisionListener* pListener)
{
	this->pListener = pListener;
}

bool ACollider::getIsTrigger() const
{
	return this->bIsTrigger;
}

void ACollider::setIsTrigger(bool bIsTrigger)
{
	this->bIsTrigger = bIsTrigger;
}

bool ACollider::hasCollided(ACollider* pCollider)
{
	if (this->findCollider(pCollider) != -1)
		return true;

	return false;
}

void ACollider::setCollided(ACollider* pCollider, bool bCollided)
{
	if (bCollided)
		this->vecCollided.push_back(pCollider);

	else {
		int nIndex = this->findCollider(pCollider);

		if (nIndex != -1)
			this->vecCollided.erase(this->vecCollided.begin() + nIndex);
	}
}

bool ACollider::isCleanUp()
{
	return this->bCleanUp;

}

void ACollider::setCleanUp(bool bCleanUp)
{
	this->bCleanUp = bCleanUp;
}

