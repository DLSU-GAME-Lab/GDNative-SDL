#pragma once
#include "AGeneralInput.h"
class PlayerInput : public AGeneralInput
{
private:
	bool bHoldingLeft;
	bool bHoldingRight;
	bool bHoldingDown;
	bool bHoldingUp;

	bool bInteracted;
	bool bJumped;
	Vector2D movement;

public:
	PlayerInput();
	~PlayerInput();

	void perform() override;

	Vector2D getMovement() const;
	bool getJumped() const;
	bool getInteracted() const;

    void setVirtualMovement(const Vector2D& v);
    void setVirtualJump(bool pressed);

    // virtual (touch) input
    Vector2D virtualMovement = Vector2D::Zero();
    bool virtualJump = false;

private:
	void onKeyDown(SDL_Keycode key);
	void onKeyUp(SDL_Keycode key);

	void updateValues();
};

