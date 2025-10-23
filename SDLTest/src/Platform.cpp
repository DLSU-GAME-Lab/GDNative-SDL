#include "Platform.h"
#include "Settings.h"
#include "PlayerController.h"
#include "ColliderRenderer.h"
Platform::Platform(const std::string& strName, Vector2D fVecTranslate, Vector2D fVecSize, float fRot) :AGameObject(strName)
{

	this->bounds = SDL_FRect{ fVecTranslate.x, fVecTranslate.y, fVecSize.x, fVecSize.y };
	this->fVecTranslate = fVecTranslate;
	this->fVecScale = Vector2D(1, 1);
}
Platform::~Platform()
{
	AGameObject::~AGameObject();
}

void Platform::initialize()
{
	this->bounds.x = fVecTranslate.x;
	this->bounds.y = fVecTranslate.y;
	BoxCollider* pCollider = new BoxCollider(this->strName + " Collider",this->bounds, true);
	pCollider->setListener(this);
	this->attachComponent((AComponent*)pCollider);
	PhysicsManager::getInstance()->trackCollider(pCollider);

	ColliderRenderer* pColRenderer = new ColliderRenderer(pCollider->getGlobalBounds());
	this->attachComponent(pColRenderer);
}



void Platform::onCollisionEnter(ACollider* pCollider)
{
	if(pCollider->getOwner()->getName().find("Player") != std::string::npos)
	{
		Gravity* pGrav = (Gravity*)pCollider->getOwner()->findComponentByName("Gravity");
		PlayerController* pController = (PlayerController*)pCollider->getOwner()->findComponentByName("PlayerController");

		SDL_FRect playerBounds = pCollider->getGlobalBounds();
		ACollider* pCol = (ACollider*)this->findComponentByName(this->strName + " Collider");

		Vector2D newPos = pCollider->getOwner()->getPos();
	

		
		if (pCollider->isCollidedBottom())
		{
			std::cout << "Top Collision" << std::endl;

		}
		else if (pCollider->isCollidedTop())
		{
			std::cout << "Bot Collision" << std::endl;

		}
		if (pCollider->isCollidedLeft())
		{
			std::cout << "Right Collision" << std::endl;

		}
		else if (pCollider->isCollidedRight())
		{
			std::cout << "Left Collision" << std::endl;
		}

		//pCollider->getOwner()->setPos(newPos);

	}

}

void Platform::onCollisionContinue(ACollider* pCollider)
{



}

void Platform::onCollisionExit(ACollider* pCollider)
{
	std::cout << "Collision Exited" << std::endl;

}


