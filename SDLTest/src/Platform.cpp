#include "Platform.h"
#include "Settings.h"
#include "PlayerController.h"
#include "ColliderRenderer.h"
Platform::Platform(const std::string& strName, Vector2D fVecTranslate, Vector2D fVecSize, float fRot) :AGameObject(strName)
{
	this->fVecTranslate = fVecTranslate;
	this->fVecScale = Vector2D(1, 1);
	this->fVecSize = fVecSize;
}
Platform::~Platform()
{
	AGameObject::~AGameObject();
}

void Platform::initialize()
{
	BoxCollider* pCollider = new BoxCollider(this->strName + " Collider");
	pCollider->setSize(this->fVecSize);
	pCollider->setListener(this);
	this->attachComponent((AComponent*)pCollider);

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


