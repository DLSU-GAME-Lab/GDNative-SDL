#pragma once
#include "AComponent.h"
#include "Vector2D.h"

class CameraController : public AComponent
{
private:
	Vector2D lastPos;
	Vector2D velocity;

	Vector2D offset;
	Vector2D lookAhead;
	float fFollowDelay;

public:
	CameraController();
	~CameraController();

	void onAttach() override;
	void perform() override;

public:
	void setOffset(Vector2D offset);
	void setLookAhead(Vector2D lookAhead);
	void setFollowDelay(float fFollowDelay);
};

