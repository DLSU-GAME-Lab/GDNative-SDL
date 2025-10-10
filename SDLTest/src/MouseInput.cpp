#include "MouseInput.h"
#include "RenderSystem.h"
#include "Camera.h"

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
	
	for (auto pListener : this->vecListener)
	{
		if (eEvent->type == SDL_EVENT_MOUSE_MOTION)
		{
			pListener->onMouseHovered(Vector2D(eEvent->motion.x, eEvent->motion.y));
		}
		else
		{
			Vector2D mousePos = Vector2D(eEvent->button.x, eEvent->button.y);
			if (this->contains(pListener->getRect(), mousePos))
			{
				switch (eEvent->type)
				{
				case SDL_EVENT_MOUSE_BUTTON_DOWN:
					pListener->onMouseButtonDown(eEvent->button.button);
					break;

				case SDL_EVENT_MOUSE_BUTTON_UP:
					pListener->onMouseButtonUp(eEvent->button.button);
					break;

				default:
					break;
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
