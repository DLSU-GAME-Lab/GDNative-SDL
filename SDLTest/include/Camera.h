#pragma once
#include "AComponent.h"
#include "Vector2D.h"

class Camera
{
private:
	Vector2D position;
	float rotation;
	Vector2D scale;
	Vector2D windowSize;
    Vector2D windowScale;

public:
	Camera();
	~Camera();

	bool isInView(const SDL_FRect& worldRect) const;
	Vector2D screenToWorldPoint(const Vector2D& screenPoint) const;
	Vector2D worldToScreenPoint(const Vector2D& worldPoint) const;

	SDL_FRect screenToWorldRect(const SDL_FRect& screenRect) const;
	SDL_FRect worldToScreenRect(const SDL_FRect& worldRect) const;

	Vector2D getWindowSize() const;
    Vector2D getWindowScale() const;
    Vector2D getWindowSizeScaled() const;
    Vector2D getWindowOffset() const;
	float getHalfWidth() const;
	float getHalfHeight() const;

	Vector2D getPos();
	float getRot();
	Vector2D getScale();

	void setWindowSize(Vector2D windowSize);
    void setWindowScale(Vector2D windowScale);
	void setPos(Vector2D position);
	void setRot(float rotation);
	void setScale(Vector2D scale);
};

