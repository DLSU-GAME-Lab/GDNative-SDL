#include "MouseInput.h"

MouseInput::MouseInput() : AGeneralInput("Mouse Input")
{

}

MouseInput::~MouseInput()
{

}

void MouseInput::perform()
{

	switch (eEvent->type)
	{
	case SDL_EVENT_MOUSE_MOTION:
		this->onMouseHovered(eEvent->motion.x);
		break;

	case SDL_EVENT_MOUSE_BUTTON_DOWN:

		break;

	case SDL_EVENT_MOUSE_BUTTON_UP:
		break;

	default:
		break;
	}
}

void MouseInput::onMouseHovered(Vector2D mousePos)
{

}

void MouseInput::onMouseButtonDown(Uint8 mouseButton)
{

}

void MouseInput::onMouseButtonUp(Uint8 mouseButton)
{

}
