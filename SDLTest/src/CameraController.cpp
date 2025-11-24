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

void CameraController::onAttach()
{
	this->lastPos = this->pOwner->getPos();
}

void CameraController::perform()
{
	Camera* pCam = CameraManager::getInstance()->getCurrentCamera();
	Vector2D posDelta = this->pOwner->getPos() - this->lastPos;

	//if (posDelta.x > 0.0f) posDelta.x += lookAhead.x;
	//else if (posDelta.x < 0.0f) posDelta.x -= lookAhead.x;
	//if (posDelta.y > 0.0f) posDelta.y += lookAhead.y;
	//else if (posDelta.y < 0.0f) posDelta.y -= lookAhead.y;

	Vector2D followPos = this->lastPos + (this->fDeltaTime * posDelta);
	
	pCam->setPos(followPos + this->offset);
	this->lastPos = followPos;
}

void CameraController::setOffset(Vector2D offset)
{
	this->offset = offset;
}

void CameraController::setLookAhead(Vector2D lookAhead)
{
	this->lookAhead = lookAhead;
}

void CameraController::setFollowDelay(float fFollowDelay)
{
	this->fFollowDelay = fFollowDelay;
}
