#include "Platform.h"
#include "Settings.h"
#include "PlayerController.h"
#include "ColliderRenderer.h"
Platform::Platform(const std::string& strName, Vector2D fVecTranslate, Vector2D fVecSize, float fRot) :AGameObject(strName)
{
	this->strImageName = strImageName;
	Vector2D tempVec = CameraManager::getInstance()->getCurrentCamera()->screenToWorldPoint(fVecTranslate);
	this->bounds = SDL_FRect{ tempVec.x, tempVec.y, fVecSize.x, fVecSize.y };
	this->fVecTranslate = fVecTranslate;
	this->fVecScale = Vector2D(1, 1);
}
Platform::~Platform()
{
	Collider* pCollider = new Collider(this->strName + " Collider", true);
	pCollider->setCleanUp(true);
	PhysicsManager::getInstance()->cleanUp();
}

void Platform::initialize()
{
	this->Rect = new RectangleShape(this->bounds.w, this->bounds.h);

	Collider* pCollider = new Collider(this->strName + " Collider", true);
	pCollider->setListener(this);
	this->attachComponent((AComponent*)pCollider);
	PhysicsManager::getInstance()->trackCollider(pCollider);
	
	ColliderRenderer* pColRenderer = new ColliderRenderer(pCollider->getGlobalBounds());
	this->attachComponent(pColRenderer);
}

SDL_FRect Platform::getGlobalBounds()
{
	return this->Rect->getGlobalBounds();
}

void Platform::onCollisionEnter(Collider* pCollider)
{
	if(pCollider->getOwner()->getName().find("Player") != std::string::npos)
	{
		Vector2D newPos = pCollider->getOwner()->getPos();
		Gravity* pGrav = (Gravity*)pCollider->getOwner()->findComponentByName("Gravity");
		Collider* pHolder = (Collider*)this->findComponentByName(this->strName + " Collider");
		PlayerController* pController = (PlayerController*)pCollider->getOwner()->findComponentByName("PlayerController");
		std::cout << "X: " << newPos.x << "Y: " << newPos.y << std::endl;


		if (pCollider->isCollidedBottom())
		{
			//turning them from world pos 
			float fTopPush = ((this->getPos().y ) + pHolder->getGlobalBounds().h / 2) + (pCollider->getOwner()->getPos().x ) / 1.1f;
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
			float fRightPush = (this->getPos().x + 1000) + (pCollider->getOwner()->getPos().x + 1000) ;
			newPos.x = fRightPush - 1000;
		}
		else if (pCollider->isCollidedRight())
		{
			//turning them from world pos 
			float fLeftPush = ((this->getPos().x ) - pHolder->getGlobalBounds().w / 2) - (pCollider->getOwner()->getPos().x ) / 10.f;
			newPos.x = fLeftPush ;

		}
		std::cout << "X: " << this->getPos().x << "Y: " << this->getPos().y << std::endl;
		std::cout << "X: " << newPos.x << "Y: " << newPos.y << std::endl;
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


