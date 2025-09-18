#pragma once
#include "AComponent.h"
#include "Vector2D.h"

class Camera
{
private:
	Vector2D position;
	float rotation;
	Vector2D scale;
public:
	Camera();
	~Camera();

	Vector2D screenToWorldPoint(Vector2D screenPoint);
	Vector2D worldToScreenPoint(Vector2D worldPoint);

	Vector2D getPos();
	float getRot();
	Vector2D getScale();

	void setPos(Vector2D position);
	void setRot(float rotation);
	void setScale(Vector2D scale);
};

