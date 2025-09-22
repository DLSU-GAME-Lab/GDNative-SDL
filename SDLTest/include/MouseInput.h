#pragma once
#include "AGeneralInput.h"
class MouseInput : public AGeneralInput
{
private:
	Vector2D mousePos;

public:
	MouseInput();
	~MouseInput();

	void perform() override;

private:
	virtual void onMouseHovered(Vector2D mousePos);
	virtual void onMouseButtonDown(Uint8 mouseButton);
	virtual void onMouseButtonUp(Uint8 mouseButton);

	bool contains(float fX, float fY);
};

