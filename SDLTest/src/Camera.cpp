#include "Camera.h"
#include "AGameObject.h"
#include "MathUtils.h"
#include "Settings.h"

Camera::Camera()
{
	this->position = Vector2D(0.0f, 0.0f);
	this->rotation = 0.0f;
	this->scale = Vector2D(1.0f, 1.0f);
}

Camera::~Camera()
{

}

bool Camera::isInView(const SDL_FRect& worldRect) const
{
	SDL_FRect camView = { 0, 0, gameWidth, gameHeight };

	return SDL_HasRectIntersectionFloat(&camView, &worldRect);
}

Vector2D Camera::screenToWorldPoint(const Vector2D& screenPoint) const
{
    Vector2D windowOffset = getWindowOffset();
    float radians = MathUtils::toRadians(this->rotation);

    // Undo screen-center + window offset translation
    float x = screenPoint.x - this->getHalfWidth() - windowOffset.x;
    float y = screenPoint.y - this->getHalfHeight() - windowOffset.y;

    // Undo rotation (apply inverse rotation)
    float unrotatedX = (x * std::cosf(-radians)) - (y * std::sinf(-radians));
    float unrotatedY = (x * std::sinf(-radians)) + (y * std::cosf(-radians));

    // Undo Y flip and scale, then translate by camera position
    Vector2D worldPoint;
    worldPoint.x = (unrotatedX * this->scale.x) + this->position.x;
    worldPoint.y = (-unrotatedY * this->scale.y) + this->position.y;

    return worldPoint;
}

Vector2D Camera::worldToScreenPoint(const Vector2D& worldPoint) const
{
    Vector2D screenPoint = (worldPoint - this->position) / this->scale;
    float radians = MathUtils::toRadians(this->rotation);

    // Flip Y before rotation
    screenPoint.y = -screenPoint.y;

    // Rotate around screen center
    float rotatedX = (screenPoint.x * std::cosf(radians)) - (screenPoint.y * std::sinf(radians));
    float rotatedY = (screenPoint.x * std::sinf(radians)) + (screenPoint.y * std::cosf(radians));

    // Translate to screen center + window offset
    screenPoint.x = rotatedX + this->getHalfWidth() + getWindowOffset().x;
    screenPoint.y = rotatedY + this->getHalfHeight() + getWindowOffset().y;

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

Vector2D Camera::getWindowSize() const
{
	return this->windowSize;
}

Vector2D Camera::getWindowScale() const
{
    return this->windowScale;
}

Vector2D Camera::getWindowSizeScaled() const
{
    return this->windowSize * this->windowScale;
}

Vector2D Camera::getWindowOffset() const
{
    return (windowSize - getWindowSizeScaled()) * 0.5f;
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

void Camera::setWindowScale(Vector2D windowScale)
{
    this->windowScale = windowScale;
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
