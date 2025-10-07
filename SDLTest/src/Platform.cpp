#include "Platform.h"
#include "Settings.h"
Platform::Platform(const std::string& strName, const std::string& strImageName, Vector2D fVecTranslate, Vector2D fVecScale, float fRot):AGameObject(strName)
{
	this->strImageName = strImageName;
	this->fVecTranslate = fVecTranslate;
	this->fVecScale = fVecScale;
}

Platform::~Platform()
{
	Collider* pCollider = new Collider(this->strName + " Collider", true);
	pCollider->setCleanUp(true);
	PhysicsManager::getInstance()->cleanUp();
}

void Platform::initialize()
{
	SpriteRenderer* pSpriteRenderer = new SpriteRenderer(this->strImageName, this->fVecTranslate.x, this->fVecTranslate.y);
	this->attachComponent((AComponent*)pSpriteRenderer);
	Collider* pCollider = new Collider(this->strName + " Collider", true);

	pCollider->setListener(this);
	this->attachComponent((AComponent*)pCollider);
	PhysicsManager::getInstance()->trackCollider(pCollider);
	

}

void Platform::onCollisionEnter(Collider* pCollider)
{
	Vector2D newPos = pCollider->getOwner()->getPos();

	if (pCollider->isCollidedBottom())
	{
		newPos.y = this->getPos().y - pCollider->getOwner()->getPos().y / 2 + 125;
	}
	else if (pCollider->isCollidedTop())
	{
		newPos.y = this->getPos().y + pCollider->getOwner()->getPos().y / 2 - 260;

		if (abs(newPos.y - this->getPos().y) > 0.1f)
			pCollider->getOwner()->setPos(newPos);


	}
	if (pCollider->isCollidedLeft())
	{
		newPos.x = (this->getPos().x - pCollider->getOwner()->getPos().x / 5) + 500;
	}
	else if (pCollider->isCollidedRight())
	{
		newPos.x = (this->getPos().x + pCollider->getOwner()->getPos().x / 5)-450;
	}
	
	pCollider->getOwner()->setPos(newPos);

}

void Platform::onCollisionContinue(Collider* pCollider)
{
	Collider* pHolder = (Collider*)this->findComponentByName(this->strName + " Collider"); 
	Gravity* pGrav = (Gravity*)pCollider->getOwner()->findComponentByName("Gravity");
	SDL_FRect platformBounds = pHolder->getGlobalBounds();
	SDL_FRect objectBounds = pCollider->getGlobalBounds();


	Vector2D newPos = pCollider->getOwner()->getPos();
	
	 if (pCollider->isCollidedBottom())
	 {
		//std::cout << newPos.y << std::endl;
		pGrav->setGrounded(true);

	 }

	pCollider->getOwner()->setPos(newPos);


}

void Platform::onCollisionExit(Collider* pCollider)
{
	std::cout << "Collision Exited" << std::endl;

}


