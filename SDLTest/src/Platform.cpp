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
	if (pCollider->isCollidedLeft())
	{
		std::cout << "Left" << std::endl;

		newPos.x = (this->getPos().x - pCollider->getOwner()->getPos().x / 2) + 320;
	}
	else if (pCollider->isCollidedRight())
	{
		std::cout << "Right" << std::endl;
		newPos.x = (this->getPos().x + pCollider->getOwner()->getPos().x / 5)-450;
		std::cout << newPos.x << std::endl;

	}
	if (pCollider->isCollidedTop())
	{
		std::cout << "TOP" << std::endl;
		newPos.y = this->getPos().y + pCollider->getOwner()->getPos().y /2 - 320;

		if (abs(newPos.y - this->getPos().y) > 0.1f)
			pCollider->getOwner()->setPos(newPos);

	}
	pCollider->getOwner()->setPos(newPos);

}

void Platform::onCollisionContinue(Collider* pCollider)
{
	Collider* pHolder = (Collider*)this->findComponentByName(this->strName + " Collider"); 
	AGameObject* pColOwner =pCollider->getOwner();
	SDL_FRect platformBounds = pHolder->getGlobalBounds();
	SDL_FRect objectBounds = pCollider->getGlobalBounds();


	Vector2D newPos = pCollider->getOwner()->getPos();
	
	 if (pCollider->isCollidedBottom())
	 {
		std::cout << "BOT" << std::endl;

		//newPos.y = ((pColOwner->getPos().y) + pCollider->getGlobalBounds().h/2 + (this->getPos().y - pHolder->getGlobalBounds().h / 2)) - 300;  // push up
		newPos.y = this->getPos().y - pCollider->getOwner()->getPos().y /2 + 175;
		std::cout << newPos.y << std::endl;

	 }

	pCollider->getOwner()->setPos(newPos);


}

void Platform::onCollisionExit(Collider* pCollider)
{
	std::cout << "Collision Exited" << std::endl;

}
