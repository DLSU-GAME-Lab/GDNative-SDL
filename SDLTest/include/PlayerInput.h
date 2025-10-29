#pragma once
#include "AGeneralInput.h"
class PlayerInput : public AGeneralInput
{
private:
	bool bHoldingLeft;
	bool bHoldingRight;
	bool bHoldingDown;
	bool bHoldingUp;

	bool bJumped;
	Vector2D movement;

public:
	PlayerInput();
	~PlayerInput();

	void perform() override;

	Vector2D getMovement() const;
	bool getJumped() const;

private:
	void onKeyDown(SDL_Keycode key);
	void onKeyUp(SDL_Keycode key);

	void updateValues();
};

