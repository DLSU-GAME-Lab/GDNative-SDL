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
	BoxCollider* pCollider = new BoxCollider(this->strName + " Collider");
	pCollider->setSize(this->bounds);
	pCollider->setListener(this);
	this->attachComponent((AComponent*)pCollider);
	PhysicsSystem::getInstance()->trackCollider(pCollider);

	ColliderRenderer* pColRenderer = new ColliderRenderer(pCollider);
	this->attachComponent(pColRenderer);
}



void Platform::onCollisionEnter(ACollider* pCollider)
{

}

void Platform::onCollisionContinue(ACollider* pCollider)
{

}

void Platform::onCollisionExit(ACollider* pCollider)
{

}


