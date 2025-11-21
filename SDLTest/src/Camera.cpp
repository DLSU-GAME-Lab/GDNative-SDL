#include "Camera.h"
#include "AGameObject.h"
#include "MathUtils.h"

Camera::Camera()
{
	this->position = Vector2D(0.0f, 0.0f);
	this->rotation = 0.0f;
	this->scale = Vector2D(1.0f, 1.0f);
}

Camera::~Camera()
{

}

Vector2D Camera::screenToWorldPoint(const Vector2D& screenPoint) const
{
	Vector2D worldPoint;

	worldPoint.x = ((screenPoint.x - this->getHalfWidth()) * this->scale.x) + this->position.x;
	worldPoint.y = (-(screenPoint.y - this->getHalfHeight()) * this->scale.y) + this->position.y;
	// TODO: fix rotations

	return worldPoint;
}

Vector2D Camera::worldToScreenPoint(const Vector2D& worldPoint) const
{
	Vector2D screenPoint = (worldPoint - this->position) / this->scale;
	float radians = MathUtils::toRadians(this->rotation);

	screenPoint.x = screenPoint.x + this->getHalfWidth();
	screenPoint.y = -screenPoint.y + this->getHalfHeight();
	// TODO: fix rotations
	//screenPoint.x = (screenPoint.x * std::cosf(radians)) - (screenPoint.y * std::sinf(radians));
	//screenPoint.y = (screenPoint.y * std::sinf(radians)) + (screenPoint.x * std::cosf(radians));

	return screenPoint;
}

SDL_FRect Camera::screenToWorldRect(const SDL_FRect& screenRect) const
{
	SDL_FRect worldRect = {};
	Vector2D worldPos = screenToWorldPoint(Vector2D(screenRect.x, screenRect.y));

	worldRect.w = screenRect.w * this->scale.x;
	worldRect.h = screenRect.h * this->scale.y;
	worldRect.x = worldPos.x;
	worldRect.y = worldPos.y + worldRect.h;

	return worldRect;
}

SDL_FRect Camera::worldToScreenRect(const SDL_FRect& worldRect) const
{
	SDL_FRect screenRect = {};
	Vector2D screenPos = worldToScreenPoint(Vector2D(worldRect.x, worldRect.y));

	screenRect.w = worldRect.w / this->scale.x;
	screenRect.h = worldRect.h / this->scale.y;
	screenRect.x = screenPos.x;
	screenRect.y = screenPos.y - screenRect.h;

	return screenRect;
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
