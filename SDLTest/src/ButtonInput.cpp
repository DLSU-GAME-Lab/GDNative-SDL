#include "ButtonInput.h"
#include "CameraManager.h"

bool ButtonInput::contains(Vector2D inputPos) const
{
    if (!this->pSprite) return false;

    SDL_FRect spriteRect = this->pSprite->getRect();
    SDL_FRect pointRect = { inputPos.x, inputPos.y, 1, 1 };

    return SDL_HasRectIntersectionFloat(&spriteRect, &pointRect);
}

bool ButtonInput::contains() const
{
    if (!this->pSprite) return false;

    SDL_FRect spriteRect = this->pSprite->getRect();
    SDL_FRect pointRect = { this->mousePos.x, this->mousePos.y, 1, 1 };

    return SDL_HasRectIntersectionFloat(&spriteRect, &pointRect);
}

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

void ButtonInput::onAttach()
{

}

void ButtonInput::perform()
{
    Vector2D inputPos;
    bool hasInput = false;
    Uint8 button = SDL_BUTTON_LEFT; // default for touch

    // Handle mouse events (for desktop testing)
    if (eEvent->type == SDL_EVENT_MOUSE_MOTION)
    {
        inputPos = Vector2D(eEvent->motion.x, eEvent->motion.y);
        hasInput = true;
        this->onMouseHovered(inputPos);
    }
    else if (eEvent->type == SDL_EVENT_MOUSE_BUTTON_DOWN ||
             eEvent->type == SDL_EVENT_MOUSE_BUTTON_UP)
    {
        inputPos = Vector2D(eEvent->button.x, eEvent->button.y);
        button = eEvent->button.button;
        hasInput = true;
    }
        // Handle touch events (for Android)
    else if (eEvent->type == SDL_EVENT_FINGER_MOTION)
    {
        // Convert normalized coordinates (0.0-1.0) to logical coordinates
        Vector2D windowSize = CameraManager::getInstance()->getWindowSize();
        inputPos = Vector2D(
                eEvent->tfinger.x * windowSize.x,
                eEvent->tfinger.y * windowSize.y
        );
        hasInput = true;
        this->onMouseHovered(inputPos);
    }
    else if (eEvent->type == SDL_EVENT_FINGER_DOWN ||
             eEvent->type == SDL_EVENT_FINGER_UP)
    {
        // Convert normalized coordinates (0.0-1.0) to logical coordinates
        Vector2D windowSize = CameraManager::getInstance()->getWindowSize();
        inputPos = Vector2D(
                eEvent->tfinger.x * windowSize.x,
                eEvent->tfinger.y * windowSize.y
        );
        hasInput = true;
    }

    if (!hasInput) return;

    // Update mouse position for hover
    if (eEvent->type == SDL_EVENT_MOUSE_MOTION ||
        eEvent->type == SDL_EVENT_FINGER_MOTION)
    {
        this->onMouseHovered(inputPos);
    }

    // Handle button/touch down and up if position is within bounds
    if (this->contains(inputPos))
    {
        if (eEvent->type == SDL_EVENT_MOUSE_BUTTON_DOWN ||
            eEvent->type == SDL_EVENT_FINGER_DOWN)
        {
            this->onMouseButtonDown(button);
        }
        else if (eEvent->type == SDL_EVENT_MOUSE_BUTTON_UP ||
                 eEvent->type == SDL_EVENT_FINGER_UP)
        {
            this->onMouseButtonUp(button);
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
	Vector2D mouseWorldPos = cam->screenToWorldPoint(mousePos);
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
