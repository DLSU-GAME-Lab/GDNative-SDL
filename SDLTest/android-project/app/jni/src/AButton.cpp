#include "AButton.h"

AButton::AButton(std::string strName) : AGameObject(strName)
{

}

AButton::~AButton()
{

}

void AButton::processInput(SDL_Event eEvent)
{
    switch (eEvent.type) {
    case SDL_EVENT_MOUSE_MOTION:
        if (this->contains(eEvent.motion.x, eEvent.motion.y)) {
            this->OnHovered();
        }
        break;

    case SDL_EVENT_MOUSE_BUTTON_DOWN:
        if (this->contains(eEvent.button.x, eEvent.button.y)) {
            this->OnPressed(eEvent.button);
        }
        break;

    case SDL_EVENT_MOUSE_BUTTON_UP:
        if (this->contains(eEvent.button.x, eEvent.button.y)) {
            this->OnReleased(eEvent.button);
        }
        break;
    }
}

bool AButton::contains(float fX, float fY)
{
	if (!this->pSprite) return false;

	// get sprite rect (texture size already adjusted by SpriteRenderer)
	SDL_FRect spriteRect = this->pSprite->getRect();

	// offset by GameObject position
	spriteRect.x = this->getPosX();
	spriteRect.y = this->getPosY();

	// apply scaling
	spriteRect.w *= this->getScaleX();
	spriteRect.h *= this->getScaleY();

	// create 1x1 point rect
	SDL_FRect pointRect = { fX, fY, 1, 1 };

	return SDL_HasRectIntersectionFloat(&spriteRect, &pointRect);
}
