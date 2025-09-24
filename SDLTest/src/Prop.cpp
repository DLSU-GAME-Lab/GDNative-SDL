#include "Prop.h"


Prop::Prop(const std::string& strName, const std::string& strImageName, Vector2D fVecTranslate, Vector2D fVecScale, float fRot, bool bFlipX):
	AGameObject(strName), strImageName(strImageName),bFlipX(bFlipX)
{
	this->fVecTranslate = fVecTranslate;
	this->fVecScale = fVecScale;
	this->fRot = fRot;
}

Prop::~Prop()
{
}

void Prop::initialize()
{
	SpriteRenderer* pSpriteRenderer = new SpriteRenderer(this->strImageName, this->fVecTranslate.x, this->fVecTranslate.y);
	this->attachComponent((AComponent*)pSpriteRenderer);

	if (bFlipX)
	{
		pSpriteRenderer->setFlipX(true);
		Collider* pCollider = new Collider(this->strName + " Collider", true);
		pCollider->setListener(this);
		this->attachComponent((AComponent*)pCollider);
		PhysicsManager::getInstance()->trackCollider(pCollider);
		std::cout << pCollider->getGlobalBounds().w << ", " << pCollider->getGlobalBounds().h << std::endl;
	}

	
}

void Prop::onCollisionEnter(Collider* pCollider)
{
	std::cout << "Collision Entered" << std::endl;
}

void Prop::onCollisionContinue(Collider* pCollider)
{
	if (this->dCount < 5)
	{
		std::cout << "Currently Colliding " << 5 - dCount << std::endl;
		dCount++;
	}
}

void Prop::onCollisionExit(Collider* pCollider)
{
	std::cout << "Collision Exited" << std::endl;

}

