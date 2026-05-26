#include "PlayerInput.h"

PlayerInput::PlayerInput() : AGeneralInput("PlayerInput")
{
	this->bHoldingLeft = false;
	this->bHoldingRight = false;
	this->bHoldingDown = false;
	this->bHoldingUp = false;
	this->bJumped = false;
	this->bInteracted = false;
}

PlayerInput::~PlayerInput()
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

Vector2D PlayerInput::getMovement() const
{
	return this->movement;
}

bool PlayerInput::getJumped() const
{
    return this->bJumped || this->virtualJump;
}

bool PlayerInput::getInteracted() const
{
	return this->bInteracted;
}

void PlayerInput::setInteracted(bool interacted)
{
	this->bInteracted = interacted;
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
	case SDLK_S:
		this->bHoldingDown = true;
		break;

	case SDLK_W:
		this->bHoldingUp = true;
		break;

	case SDLK_SPACE:
		this->bJumped = true;
		break;

	case SDLK_E:
		this->bInteracted = true;
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
	case SDLK_S:
		this->bHoldingDown = false;
		break;
	case SDLK_W:
		this->bHoldingUp = false;
		break;

	case SDLK_SPACE:
		this->bJumped = false;
		break;

	case SDLK_E:
		this->bInteracted = false;
		break;

	default:
		break;
	}

	this->updateValues();
}

void PlayerInput::updateValues(bool virtualControls)
{
    float keyX = 0.0f;
    if (bHoldingLeft && !bHoldingRight) keyX = -1.0f;
    else if (bHoldingRight && !bHoldingLeft) keyX = 1.0f;
    this->movement = Vector2D(keyX, 0.0f);

    //SDL_Log("Player Movement: %.2f", this->movement.x);
    // joystick overrides keyboard when active
//    if (bHoldingLeft || bHoldingRight)
//        this->movement = Vector2D(keyX, 0.0f);
//    else
//        this->movement = this->virtualMovement;
}

void PlayerInput::setVirtualMovement(const Vector2D& v)
{
    this->virtualMovement = v;
    //SDL_Log("Virtual Movement: %.2f", this->virtualMovement.x);
    //this->updateValues(true);
}

void PlayerInput::setVirtualJump(bool pressed)
{
    this->virtualJump = pressed;
}
