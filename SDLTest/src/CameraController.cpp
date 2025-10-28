#include "CameraController.h"
#include "AGameObject.h"
#include "CameraManager.h"

CameraController::CameraController() : AComponent("CameraController", ComponentType::SCRIPT)
{
	this->fFollowDelay = 0.0f;
}

CameraController::~CameraController()
{

}

void CameraController::perform()
{
	Camera* pCam = CameraManager::getInstance()->getCurrentCamera();
	Vector2D followPos = this->pOwner->getPos();
	// TODO: add more following logic
	pCam->setPos(followPos + this->offset);
}

void CameraController::setOffset(Vector2D offset)
{
	this->offset = offset;
}

void CameraController::setFollowDelay(float fFollowDelay)
{
	this->fFollowDelay = fFollowDelay;
}
