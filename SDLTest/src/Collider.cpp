#include "Collider.h"

Collider::Collider(std::string strName, bool bFollowParent):AComponent(strName,ComponentType::SCRIPT)
{
	this->COffset = SDL_FRect(0.f, 0.f, 0.f, 0.f);
	this->vecCollided = {};
	this->bCleanUp = false;
	this->bFollowParent = bFollowParent;
}
void Collider::onAttach()
{

}
void Collider::perform()
{
	
}

bool Collider::isColliding(Collider* pCollider)
{
	SDL_FRect CBoundsA = this->getGlobalBounds();
	SDL_FRect CBoundsB = pCollider->getGlobalBounds();

	float fLeftA, fLeftB;
	float fRightA, fRightB;
	float fTopA, fTopB;
	float fBotA, fBotB;
	//RectA 
	fLeftA = CBoundsA.x;
	fRightA = CBoundsA.x + CBoundsA.w;
	fTopA = CBoundsA.y;
	fBotA = CBoundsA.y + CBoundsA.h;

	//RectA 
	fLeftB = CBoundsB.x;
	fRightB = CBoundsB.x + CBoundsB.w;
	fTopB = CBoundsB.y;
	fBotB = CBoundsB.y + CBoundsB.h;

	bool bCollisionX = (fLeftA < fRightB) && (fRightA > fLeftB);
	bool bCollisionY = (fTopA < fBotB) && (fBotA > fTopB);
	return bCollisionX && bCollisionY;
}

int Collider::findCollider(Collider* pCollider)
{
	int nIndex = -1;

	for (int i = 0; i < this->vecCollided.size() && nIndex == -1; i++) {
		if (pCollider == this->vecCollided[i])
			nIndex = i;
	}

	return nIndex;
}

void Collider::cleanCollisions()
{
	this->vecCollided.clear();
}

void Collider::onCollisionEnter(Collider* pCollider)
{
	if (this->pListener != NULL) {
		this->pListener->onCollisionEnter(pCollider);
	}
}

void Collider::onCollisionContinue(Collider* pCollider)
{
	if (this->pListener != NULL) {
		this->pListener->onCollisionContinue(pCollider);
	}
}

void Collider::onCollisionExit(Collider* pCollider)
{
	if (this->pListener != NULL) {
		this->pListener->onCollisionExit(pCollider);
	}
}

void Collider::setListener(ACollisionListener* pListener)
{
	this->pListener = pListener;
}

SDL_FRect Collider::getOffset()
{
	return this->COffset;
}

void Collider::setOffset(SDL_FRect COffset)
{
	this->COffset = COffset;
}

bool Collider::hasCollided(Collider* pCollider)
{
	if (this->findCollider(pCollider) != -1)
		return true;

	return false;
}

void Collider::setCollided(Collider* pCollider, bool bCollided)
{
	if (bCollided)
		this->vecCollided.push_back(pCollider);

	else {
		int nIndex = this->findCollider(pCollider);

		if (nIndex != -1)
			this->vecCollided.erase(this->vecCollided.begin() + nIndex);
	}
}

bool Collider::isCleanUp()
{
	return this->bCleanUp;

}

void Collider::setCleanUp(bool bCleanUp)
{
	this->bCleanUp = bCleanUp;
}

SDL_FRect Collider::getGlobalBounds()
{
	SpriteRenderer* renderer = (SpriteRenderer*)this->pOwner->findComponentByName("SpriteRenderer");
	SDL_FRect CTransform = renderer->getRect();

	CTransform.x = CTransform.x + this->COffset.x;
	CTransform.y = CTransform.y + this->COffset.y;
	CTransform.w = CTransform.w + this->COffset.w;
	CTransform.h = CTransform.h + this->COffset.h;

	return CTransform;
}
