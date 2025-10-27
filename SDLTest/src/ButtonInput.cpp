#include "ButtonInput.h"
#include "CameraManager.h"

ButtonInput::ButtonInput(SpriteRenderer* pSprite) : AGeneralInput("ButtonInput")
{
	this->pSprite = pSprite;
	this->bHolding = false;
	this->bDragging = false;
	this->bClicked = false;
	this->bRightClick = false;
	this->bLefttClick = false;
}

ButtonInput::~ButtonInput()
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
	return this->mousePos;
}

Vector2D ButtonInput::getMouseWorldPos() const
{
	Camera* cam = CameraManager::getInstance()->getCurrentCamera();
	Vector2D mouseWorldPos;

	mouseWorldPos.x = (this->mousePos.x + cam->getPos().x - cam->getHalfWidth()) * cam->getScale().x;
	mouseWorldPos.y = (-(this->mousePos.y - cam->getPos().y - cam->getHalfHeight())) * cam->getScale().y;

	return mouseWorldPos;
}

bool ButtonInput::getHolding() const
{
	return this->bHolding;
}

bool ButtonInput::getDragging() const
{
	return this->bDragging;
}

bool ButtonInput::getClicked() const
{
	return this->bClicked;
}

void ButtonInput::setClicked(bool bClicked)
{
	this->bClicked = bClicked;
}

bool ButtonInput::isRightClick()
{
	return this->bRightClick;
}

void ButtonInput::setRightClick(bool bRightClick)
{
	this->bRightClick = bRightClick;
}

bool ButtonInput::isLeftClick()
{
	return this->bLefttClick;
}

void ButtonInput::setLeftClick(bool bLeftClick)
{
	this->bLefttClick = bLeftClick;
}

void ButtonInput::onMouseHovered(Vector2D mousePos)
{
	this->mousePos = mousePos;
	if (this->bHolding) this->bDragging = true;
	else this->bDragging = false;
}

void ButtonInput::onMouseButtonDown(Uint8 mouseButton)
{
	this->bHolding = true;

}

void ButtonInput::onMouseButtonUp(Uint8 mouseButton)
{
	this->bHolding = false;
	this->bDragging = false;
	this->bClicked = true;
	if (mouseButton == SDL_BUTTON_RIGHT)
	{
		this->bRightClick = true;
	}
	else if (mouseButton == SDL_BUTTON_LEFT)
	{
		this->bLefttClick = true;
	}

}

bool ButtonInput::contains() const
{
	if (!this->pSprite) return false;

	SDL_FRect spriteRect = this->pSprite->getRect();
	SDL_FRect pointRect = { this->mousePos.x, this->mousePos.y, 1, 1 };

	return SDL_HasRectIntersectionFloat(&spriteRect, &pointRect);
}
