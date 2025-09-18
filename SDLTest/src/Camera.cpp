#include "Camera.h"
#include "AGameObject.h"

Camera::Camera()
{
	this->position = Vector2D(0.0f, 0.0f);
	this->rotation = 0.0f;
	this->scale = Vector2D(1.0f, 1.0f);
}

Camera::~Camera()
{

}

Vector2D Camera::screenToWorldPoint(Vector2D screenPoint) const
{

	Vector2D worldPoint;
	worldPoint.x = (screenPoint.x + this->position.x - this->getHalfWidth()) * this->scale.x;
	worldPoint.y = (-(screenPoint.y + this->position.y - this->getHalfHeight()) - this->windowSize.y) * this->scale.y;

	//Vector2D worldPoint;
	//worldPoint.x = (screenPoint.x + this->position.x - this->getHalfWidth()) * this->scale.x;
	//worldPoint.y = -(screenPoint.y - this->position.y - this->getHalfHeight()) * this->scale.y;

	return worldPoint;
}

Vector2D Camera::worldToScreenPoint(Vector2D worldPoint) const
{

	Vector2D screenPoint;
	screenPoint.x = (worldPoint.x - this->position.x + this->getHalfWidth()) / this->scale.x;
	screenPoint.y = (-(worldPoint.y - this->position.y + this->getHalfHeight()) + this->windowSize.y) / this->scale.y;

	//Vector2D screenPoint;
	//screenPoint.x = (worldPoint.x - this->position.x + this->getHalfWidth()) / this->scale.x;
	//screenPoint.y = -(worldPoint.y + this->position.y + this->getHalfHeight()) / this->scale.y;

	return screenPoint;
}

Vector2D Camera::getWindowSize()
{
	return this->windowSize;
}

float Camera::getHalfWidth() const
{
	return this->windowSize.x * 0.5f;
}

float Camera::getHalfHeight() const
{
	return this->windowSize.y * 0.5f;
}

Vector2D Camera::getPos()
{
	return this->position;
}

float Camera::getRot()
{
	return this->rotation;
}

Vector2D Camera::getScale()
{
	return this->scale;
}

void Camera::setWindowSize(Vector2D windowSize)
{
	this->windowSize = windowSize;
}

void Camera::setPos(Vector2D position)
{
	this->position = position;
}

void Camera::setRot(float rotation)
{
	this->rotation = rotation;
}

void Camera::setScale(Vector2D scale)
{
	this->scale = scale;
}
