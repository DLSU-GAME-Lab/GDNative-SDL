#include "AButton.h"
#include "SpriteRendererSystem.h"
#include <iostream>

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

    SDL_FRect spriteRect = this->pSprite->getRect();
    SDL_FRect pointRect = { fX, fY, 1, 1 };

    // log full info
    std::cout << "Button: " << this->getName()
        << " Rect: (" << spriteRect.x << ", " << spriteRect.y
        << ", " << spriteRect.w << ", " << spriteRect.h << ")"
        << " Mouse Logical: (" << fX << ", " << fY << ")"
        << std::endl;

    return SDL_HasRectIntersectionFloat(&spriteRect, &pointRect);
}

