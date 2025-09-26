#include "PlayerInput.h"

PlayerInput::PlayerInput() : AGeneralInput("PlayerInput")
{
	this->fMoveX = 0.0f;
	this->bJumped = false;
}

PlayerInput::~PlayerInput()
{

}

void PlayerInput::onAttach()
{

}

void PlayerInput::perform()
{
	switch (eEvent->type)
	{
	case SDL_EVENT_KEY_DOWN:
		this->onKeyDown(eEvent->key.key);
		break;

	case SDL_EVENT_KEY_UP:
		this->onKeyUp(eEvent->key.key);
		break;

	default:
		break;
	}
}

float PlayerInput::getMoveX() const
{
	return this->fMoveX;
}

bool PlayerInput::getJumped() const
{
	return this->bJumped;
}

void PlayerInput::onKeyDown(SDL_Keycode key)
{
	switch (key)
	{
	case SDLK_A:
		this->bHoldingLeft = true;
		break;

	case SDLK_D:
		this->bHoldingRight = true;
		break;

	case SDLK_SPACE:
		this->bJumped = true;
		break;

	default:
		break;
	}

	this->updateValues();
}

void PlayerInput::onKeyUp(SDL_Keycode key)
{
	switch (key)
	{
	case SDLK_A:
		this->bHoldingLeft = false;
		break;
	case SDLK_D:

		this->bHoldingRight = false;
		break;

	case SDLK_SPACE:
		this->bJumped = false;
		break;

	default:
		break;
	}

	this->updateValues();
}

void PlayerInput::updateValues()
{
	if (this->bHoldingLeft && this->bHoldingRight) this->fMoveX = 0.0f;
	else if (this->bHoldingLeft) this->fMoveX = -1.0f;
	else if (this->bHoldingRight) this->fMoveX = 1.0f;
	else this->fMoveX = 0.0f;
}
