#include "MouseInput.h"
#include "Camera.h"
#include "CameraManager.h"

MouseInput::MouseInput() : AGeneralInput("MouseInput")
{

}

MouseInput::~MouseInput()
{

}

void MouseInput::onAttach()
{

}

void MouseInput::perform()
{
    Vector2D inputPos;
    Uint8 button = SDL_BUTTON_LEFT;

    // Handle mouse events
    if (eEvent->type == SDL_EVENT_MOUSE_MOTION)
    {
        inputPos = Vector2D(eEvent->motion.x, eEvent->motion.y);
    }
    else if (eEvent->type == SDL_EVENT_MOUSE_BUTTON_DOWN ||
             eEvent->type == SDL_EVENT_MOUSE_BUTTON_UP)
    {
        inputPos = Vector2D(eEvent->button.x, eEvent->button.y);
        button = eEvent->button.button;
    }
        // Handle touch events
    else if (eEvent->type == SDL_EVENT_FINGER_MOTION)
    {
        Vector2D windowSize = CameraManager::getInstance()->getWindowSize();
        inputPos = Vector2D(
                eEvent->tfinger.x * windowSize.x,
                eEvent->tfinger.y * windowSize.y
        );
    }
    else if (eEvent->type == SDL_EVENT_FINGER_DOWN ||
             eEvent->type == SDL_EVENT_FINGER_UP)
    {
        Vector2D windowSize = CameraManager::getInstance()->getWindowSize();
        inputPos = Vector2D(
                eEvent->tfinger.x * windowSize.x,
                eEvent->tfinger.y * windowSize.y
        );
        button = SDL_BUTTON_LEFT; // Treat touch as left click
    }
    else
    {
        return; // Event type not handled
    }

    for (auto pListener : this->vecListener)
    {
        if (eEvent->type == SDL_EVENT_MOUSE_MOTION ||
            eEvent->type == SDL_EVENT_FINGER_MOTION)
        {
            pListener->onMouseHovered(inputPos);
        }
        else
        {
            if (this->contains(pListener->getRect(), inputPos))
            {
                if (eEvent->type == SDL_EVENT_MOUSE_BUTTON_DOWN ||
                    eEvent->type == SDL_EVENT_FINGER_DOWN)
                {
                    pListener->onMouseButtonDown(button);
                }
                else if (eEvent->type == SDL_EVENT_MOUSE_BUTTON_UP ||
                         eEvent->type == SDL_EVENT_FINGER_UP)
                {
                    pListener->onMouseButtonUp(button);
                }
            }
        }
    }
}

void MouseInput::addListener(IMouseInputListener* pListener)
{
	this->vecListener.push_back(pListener);
}

void MouseInput::removeListener(IMouseInputListener * pListener)
{
	int nIndex = -1;
	for (int i = 0; i < this->vecListener.size() && nIndex == -1; i++)
	{
		if (this->vecListener[i] == pListener) nIndex = i;
	}

	if (nIndex != -1) this->vecListener.erase(this->vecListener.begin() + nIndex);
}

bool MouseInput::contains(SDL_FRect spriteRect, Vector2D mousePos) const
{
	SDL_FRect pointRect = { mousePos.x, mousePos.y, 1, 1 };
	return SDL_HasRectIntersectionFloat(&spriteRect, &pointRect);
}
