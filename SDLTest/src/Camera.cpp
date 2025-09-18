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

Vector2D Camera::screenToWorldPoint(Vector2D screenPoint)
{
	return Vector2D();
}

Vector2D Camera::worldToScreenPoint(Vector2D worldPoint)
{
	return Vector2D();
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
