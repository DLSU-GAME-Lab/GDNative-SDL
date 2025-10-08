#include "Platform.h"
#include "Settings.h"
#include "PlayerController.h"
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
	if(pCollider->getOwner()->getName().find("Player") != std::string::npos)
	{
		Vector2D newPos = pCollider->getOwner()->getPos();
		Gravity* pGrav = (Gravity*)pCollider->getOwner()->findComponentByName("Gravity");
		Collider* pHolder = (Collider*)this->findComponentByName(this->strName + " Collider");
		PlayerController* pController = (PlayerController*)pCollider->getOwner()->findComponentByName("PlayerController");

		if (pCollider->isCollidedBottom())
		{
			//turning them from world pos 
			float fTopPush = ((this->getPos().y + 600) + pHolder->getGlobalBounds().h / 2) + (pCollider->getOwner()->getPos().x + 600) / 1.1f;
			newPos.y = fTopPush - 600;
			pGrav->setGrounded(true);

		}
		else if (pCollider->isCollidedTop())
		{
			//turning them from world pos 
			float fBotPush = ((this->getPos().y + 600)) - (pCollider->getOwner()->getPos().x + 600) / 1.3f;
			newPos.y = fBotPush - 600;
			pController->setVelY(0);
		}
		if (pCollider->isCollidedLeft())
		{
			//turning them from world pos 
			float fRightPush = (this->getPos().x + 1000) + (pCollider->getOwner()->getPos().x + 1000) / 2.6;
			newPos.x = fRightPush - 1000;
		}
		else if (pCollider->isCollidedRight())
		{
			//turning them from world pos 
			float fLeftPush = ((this->getPos().x + 1000) - pHolder->getGlobalBounds().w / 2) - (pCollider->getOwner()->getPos().x + 1000) / 1.8f;
			newPos.x = fLeftPush - 1000;

		}

		pCollider->getOwner()->setPos(newPos);
	}

}

void Platform::onCollisionContinue(Collider* pCollider)
{



}

void Platform::onCollisionExit(Collider* pCollider)
{
	std::cout << "Collision Exited" << std::endl;

}


