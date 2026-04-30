#include "ButtonInput.h"
#include "CameraManager.h"

ButtonInput::ButtonInput(SpriteRenderer* pSprite) : AGeneralInput("ButtonInput")
{
	this->pSprite = pSprite;
	this->bHolding = false;
	this->bDragging = false;
	this->bClicked = false;
	this->bRightClick = false;
	this->bLeftClick = false;
}

ButtonInput::~ButtonInput()
{

}

void ButtonInput::perform()
{
    if (!eEvent) {
        std::cout << "[ButtonInput] NO eEvent set on component\n";
    } else {
        std::cout << "[ButtonInput] eEvent type=" << eEvent->type
                  << " logical=(" << this->logicalX << "," << this->logicalY << ")\n";
    }

    // Hover: mouse or finger motion -> use logical coords (AGeneralInput stores them)
    if (eEvent->type == SDL_EVENT_MOUSE_MOTION || eEvent->type == SDL_EVENT_FINGER_MOTION)
    {
        this->onMouseHovered(Vector2D(this->logicalX, this->logicalY));
    }

    // Use logical coords converted by AGeneralInput
    // (AGeneralInput::setEvent sets logicalX/logicalY)
    this->mousePos = Vector2D(this->logicalX, this->logicalY);
    // hover handling (will set dragging if holding)
    this->onMouseHovered(this->mousePos);

    // For click/press we accept both mouse and finger events
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

            case SDL_EVENT_FINGER_DOWN:
                // treat as left mouse down
                this->onMouseButtonDown(SDL_BUTTON_LEFT);
                break;

            case SDL_EVENT_FINGER_UP:
                // treat as left mouse up
                this->onMouseButtonUp(SDL_BUTTON_LEFT);
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
	return this->bLeftClick;
}

void ButtonInput::setLeftClick(bool bLeftClick)
{
	this->bLeftClick = bLeftClick;
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
		this->bLeftClick = true;
	}

}

/*
bool ButtonInput::contains() const
{
	if (!this->pSprite) return false;

	SDL_FRect spriteRect = this->pSprite->getRect();
	SDL_FRect pointRect = { this->mousePos.x, this->mousePos.y, 1, 1 };

	return SDL_HasRectIntersectionFloat(&spriteRect, &pointRect);
}
*/
// call with no-arg: use the stored mousePos
bool ButtonInput::contains() const
{
    return contains(this->mousePos);
}

// call with explicit point
bool ButtonInput::contains(const Vector2D& pos) const
{
    if (!this->pSprite) return false;

    SDL_FRect spriteRect = this->pSprite->getRect();
    SDL_FRect pointRect = { pos.x, pos.y, 1, 1 };

    bool intersect = SDL_HasRectIntersectionFloat(&spriteRect, &pointRect);

    // debug: enable temporarily to inspect values
    std::cout << "[ButtonInput] mousePos=(" << pos.x << "," << pos.y << ") spriteRect=(x="
    << spriteRect.x << ",y=" << spriteRect.y << ",w=" << spriteRect.w << ",h=" << spriteRect.h << ") -> " << intersect << std::endl;

    return intersect;
}


