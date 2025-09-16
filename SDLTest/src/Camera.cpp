#include "Camera.h"
#include "AGameObject.h"

Camera::Camera(std::string strName) : AComponent(strName, ComponentType::CAMERA)
{

}

Camera::~Camera()
{

}

void Camera::perform()
{

}

Vector2D Camera::getPos()
{
	return this->pOwner->getPos();
}

float Camera::getRot()
{
	return this->pOwner->getRot();
}

Vector2D Camera::getScale()
{
	return this->pOwner->getScale();
}
