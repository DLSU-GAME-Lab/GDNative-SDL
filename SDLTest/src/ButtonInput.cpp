#include "ButtonInput.h"
#include "RenderSystem.h"
#include "Camera.h"

ButtonInput::ButtonInput(SpriteRenderer* pSprite) : AGeneralInput("ButtonInput")
{
	this->pSprite = pSprite;
	this->bHolding = false;
	this->bClicked = false;
}

ButtonInput::~ButtonInput()
{
	this->bHolding = false;
	this->bClicked = false;
}

void ButtonInput::onAttach()
{

}

void ButtonInput::perform()
{
	if (eEvent->type == SDL_EVENT_MOUSE_MOTION)
		this->onMouseHovered(Vector2D(eEvent->motion.x, eEvent->motion.y));

	if (this->contains())
	{
		switch (eEvent->type)
		{
		case SDL_EVENT_MOUSE_BUTTON_DOWN:
			this->onMouseButtonDown(eEvent->button.button);
			break;

		case SDL_EVENT_MOUSE_BUTTON_UP:
			this->onMouseButtonUp(eEvent->button.button);
			break;

		default:
			break;
		}
	}
}

Vector2D ButtonInput::getMousePos() const
{
	return Vector2D();
}

Vector2D ButtonInput::getMouseWorldPos() const
{
	Camera* cam = RenderSystem::getInstance()->getCamera();
	Vector2D mouseWorldPos;

	mouseWorldPos.x = (this->mousePos.x + cam->getPos().x - cam->getHalfWidth()) * cam->getScale().x;
	mouseWorldPos.y = (-(this->mousePos.y - cam->getPos().y - cam->getHalfHeight())) * cam->getScale().y;

	return mouseWorldPos;
}

bool ButtonInput::getHolding() const
{
	return this->bHolding;
}

bool ButtonInput::getClicked() const
{
	return this->bClicked;
}

void ButtonInput::setClicked(bool bClicked)
{
	this->bClicked = bClicked;
}

void ButtonInput::onMouseHovered(Vector2D mousePos)
{
	this->mousePos = mousePos;
}

void ButtonInput::onMouseButtonDown(Uint8 mouseButton)
{
	this->bHolding = true;
}

void ButtonInput::onMouseButtonUp(Uint8 mouseButton)
{
	this->bHolding = false;
	this->bClicked = true;
}

bool ButtonInput::contains() const
{
	if (!this->pSprite) return false;

	SDL_FRect spriteRect = this->pSprite->getRect();
	SDL_FRect pointRect = { this->mousePos.x, this->mousePos.y, 1, 1 };

	return SDL_HasRectIntersectionFloat(&spriteRect, &pointRect);
}
