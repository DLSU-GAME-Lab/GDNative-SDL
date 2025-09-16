#pragma once
#include "AComponent.h"
#include "Vector2D.h"

class Camera : public AComponent
{
public:
	Camera(std::string strName);
	~Camera();

	virtual void perform() override;

	Vector2D getPos();
	float getRot();
	Vector2D getScale();
};

