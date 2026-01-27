#include "AGeneralInput.h"
#include "RendererContext.h"

AGeneralInput::AGeneralInput(std::string strName) : AComponent(strName, ComponentType::INPUT)
{
	this->eEvent = NULL;
}

AGeneralInput::~AGeneralInput()
{

}

void AGeneralInput::setEvent(SDL_Event* eEvent)
{
    this->eEvent = eEvent;

    if (!eEvent) return;

    SDL_Renderer* renderer = RendererContext::getInstance()->getRenderer();
    if (!renderer) return;

    float lx = 0.0f, ly = 0.0f;

    // Mouse motion uses motion; button events use button coords.
    if (eEvent->type == SDL_EVENT_MOUSE_MOTION)
    {
        SDL_RenderCoordinatesFromWindow(
                renderer,
                (float)eEvent->motion.x,
                (float)eEvent->motion.y,
                &lx, &ly
        );
    }
    else if (eEvent->type == SDL_EVENT_MOUSE_BUTTON_DOWN ||
             eEvent->type == SDL_EVENT_MOUSE_BUTTON_UP)
    {
        SDL_RenderCoordinatesFromWindow(
                renderer,
                (float)eEvent->button.x,
                (float)eEvent->button.y,
                &lx, &ly
        );
    }
        // Touch (Android) events
    else if (eEvent->type == SDL_EVENT_FINGER_DOWN ||
             eEvent->type == SDL_EVENT_FINGER_UP ||
             eEvent->type == SDL_EVENT_FINGER_MOTION)
    {
        SDL_Window* window = SDL_GetRenderWindow(renderer);
        int w, h;
        SDL_GetWindowSizeInPixels(window, &w, &h);

        float winX = eEvent->tfinger.x * w;
        float winY = eEvent->tfinger.y * h;

        SDL_RenderCoordinatesFromWindow(renderer, winX, winY, &lx, &ly);
    }
    else
    {
        // For other event types, keep previous coords (do nothing)
        return;
    }

    logicalX = lx;
    logicalY = ly;
}
