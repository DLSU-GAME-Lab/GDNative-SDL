#pragma once
#include "AComponent.h"
#include "Vector2D.h"

class CameraController : public AComponent
{
private:
	Vector2D velocity;

	Vector2D offset;
	float fFollowDelay;

public:
	CameraController();
	~CameraController();

	void perform() override;

public:
	void setOffset(Vector2D offset);
	void setFollowDelay(float fFollowDelay);
};

