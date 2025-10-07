#include "ButtonInput.h"
#include "RenderSystem.h"
#include "Camera.h"
#include "Settings.h"
#include "imgui.h"

ButtonInput::ButtonInput(SpriteRenderer* pSprite) : AGeneralInput("ButtonInput")
{
	this->pSprite = pSprite;
	this->bHolding = false;
	this->bClicked = false;
}

ButtonInput::~ButtonInput()
{
	this->bHolding = false;
	this->bClicked = false;
}

void ButtonInput::perform()
{
    if (!eEvent) return;

    // If ImGui wants mouse, bail out (prevent game input when ImGui is active)
    if (ImGui::GetCurrentContext() != nullptr) {
        ImGuiIO& io = ImGui::GetIO();
        if (io.WantCaptureMouse) return;
    }

#if defined(__ANDROID__)
    if (eEvent->type == SDL_EVENT_FINGER_DOWN ||
        eEvent->type == SDL_EVENT_FINGER_UP ||
        eEvent->type == SDL_EVENT_FINGER_MOTION)
    {
        // After SDL_ConvertEventToRenderCoordinates, tfinger.x/y are in renderer logical coords
        float logicalX = eEvent->tfinger.x;
        float logicalY = eEvent->tfinger.y;
        Vector2D pos(logicalX, logicalY);

        SDL_FRect sRect = this->pSprite ? this->pSprite->getRect() : SDL_FRect{0,0,0,0};
        SDL_Log("pointer=(%.1f,%.1f) spriteRect=(%.1f,%.1f,%.1f,%.1f)",
                pos.x, pos.y, sRect.x, sRect.y, sRect.w, sRect.h);

        if (this->contains(pos))
        {
            this->onMouseHovered(pos);
            if (eEvent->type == SDL_EVENT_FINGER_DOWN) this->onMouseButtonDown(SDL_BUTTON_LEFT);
            else if (eEvent->type == SDL_EVENT_FINGER_UP) this->onMouseButtonUp(SDL_BUTTON_LEFT);
        }
    }
#else
    if (eEvent->type == SDL_EVENT_MOUSE_MOTION ||
        eEvent->type == SDL_EVENT_MOUSE_BUTTON_DOWN ||
        eEvent->type == SDL_EVENT_MOUSE_BUTTON_UP)
    {
        // After conversion, motion.x/y are in renderer logical coords
        float logicalX = static_cast<float>(eEvent->motion.x);
        float logicalY = static_cast<float>(eEvent->motion.y);
        Vector2D pos(logicalX, logicalY);

        if (this->contains(pos))
        {
            this->onMouseHovered(pos);
            if (eEvent->type == SDL_EVENT_MOUSE_BUTTON_DOWN) this->onMouseButtonDown(eEvent->button.button);
            else if (eEvent->type == SDL_EVENT_MOUSE_BUTTON_UP) this->onMouseButtonUp(eEvent->button.button);
        }
    }
#endif
}

Vector2D ButtonInput::getMousePos() const
{
	return Vector2D();
}

Vector2D ButtonInput::getMouseWorldPos() const
{
	Camera* cam = RenderSystem::getInstance()->getCamera();
	Vector2D mouseWorldPos;

	mouseWorldPos.x = (this->mousePos.x + cam->getPos().x - cam->getHalfWidth()) * cam->getScale().x;
	mouseWorldPos.y = (-(this->mousePos.y - cam->getPos().y - cam->getHalfHeight())) * cam->getScale().y;

	return mouseWorldPos;
}

bool ButtonInput::getHolding() const
{
	return this->bHolding;
}

bool ButtonInput::getClicked() const
{
	return this->bClicked;
}

void ButtonInput::setClicked(bool bClicked)
{
	this->bClicked = bClicked;
}

void ButtonInput::onMouseHovered(Vector2D mousePos)
{
	this->mousePos = mousePos;
}

void ButtonInput::onMouseButtonDown(Uint8 mouseButton)
{
	this->bHolding = true;
}

void ButtonInput::onMouseButtonUp(Uint8 mouseButton)
{
	this->bHolding = false;
	this->bClicked = true;
}

bool ButtonInput::contains(const Vector2D& pos) const
{
	if (!this->pSprite) return false;

	SDL_FRect spriteRect = this->pSprite->getRect();
    SDL_FRect pointRect = { pos.x, pos.y, 1, 1 };

	return SDL_HasRectIntersectionFloat(&spriteRect, &pointRect);
}
