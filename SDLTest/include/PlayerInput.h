#pragma once
#include "AGeneralInput.h"
class PlayerInput : public AGeneralInput
{
private:
	bool bHoldingLeft;
	bool bHoldingRight;
	bool bJumped;
	float fMoveX;

public:
	PlayerInput();
	~PlayerInput();

	void perform() override;

	float getMoveX() const;
	bool getJumped() const;

private:
	void onKeyDown(SDL_Keycode key);
	void onKeyUp(SDL_Keycode key);

	void updateValues();
};

