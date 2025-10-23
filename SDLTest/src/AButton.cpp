#include "AButton.h"
#include <iostream>

AButton::AButton(std::string strName) : AGameObject(strName)
{
    this->pSprite = NULL;
}

void AButton::processInput(SDL_Event* eEvent)
{
    if (this->contains(eEvent->motion.x, eEvent->motion.y))
    {
        switch (eEvent->type)
        {
        case SDL_EVENT_MOUSE_MOTION:
            this->OnHovered();
            break;

        case SDL_EVENT_MOUSE_BUTTON_DOWN:
            this->OnPressed(eEvent->button);
            break;

        case SDL_EVENT_MOUSE_BUTTON_UP:
            this->OnReleased(eEvent->button);
            break;
        }
    }
}

bool AButton::contains(float fX, float fY)
{
    if (!this->pSprite) return false;

    SDL_FRect spriteRect = this->pSprite->getRect();
    SDL_FRect pointRect = { fX, fY, 1, 1 };

    //// log full info
    //std::cout << "Button: " << this->getName()
    //    << " Rect: (" << spriteRect.x << ", " << spriteRect.y
    //    << ", " << spriteRect.w << ", " << spriteRect.h << ")"
    //    << " Mouse Logical: (" << fX << ", " << fY << ")"
    //    << std::endl;

    return SDL_HasRectIntersectionFloat(&spriteRect, &pointRect);
}

