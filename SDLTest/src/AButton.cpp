#include "AButton.h"

AButton::AButton(std::string strName) : AGameObject(strName)
{

}

AButton::~AButton()
{

}

void AButton::processInput(SDL_Event eEvent)
{
	if (eEvent.motion.xrel != 0 || eEvent.motion.yrel != 0)
	{
		this->OnHovered();
	}
	
	if (this->contains(eEvent.button.x, eEvent.button.y))
	{
		switch (eEvent.button.type)
		{
		case SDL_EVENT_MOUSE_BUTTON_DOWN:
			this->OnPressed(eEvent.button);
			break;

		case SDL_EVENT_MOUSE_BUTTON_UP:
			this->OnReleased(eEvent.button);
			break;

		default:
			break;
		}
	}
}

bool AButton::contains(float fX, float fY)
{
	SDL_FRect spriteRect = this->pSprite->getRect();
	SDL_FRect pointRect = { fX, fY, 1, 1 };

	if (SDL_HasRectIntersectionFloat(&spriteRect, &pointRect))
		return true;

	return false;
}
