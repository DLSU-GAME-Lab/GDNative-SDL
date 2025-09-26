#pragma once
#include "AComponent.h"
#include "PlayerInput.h"
#include "SpriteRenderer.h"

class PlayerController : public AComponent
{
private:
	PlayerInput* pInput;
	SpriteRenderer* pSprite;
	float fMoveSpeed;
	float fJumpForce;

public:
	PlayerController(PlayerInput* pInput, SpriteRenderer* pSprite);
	~PlayerController();

	void onAttach() override;
	void perform() override;

	void setMoveSpeed(float fMoveSpeed);
	void setJumpForce(float fJumpForce);
};

