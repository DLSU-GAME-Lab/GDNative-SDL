//
// Created by nalyr on 3/5/2025.
//
#include "TouchArea.h"

TouchArea::TouchArea(const std::string& id, SDL_Rect bounds,
                     TouchCallback callback, const std::string& scene)
        : id(id),
          bounds(bounds),
          pressed(false),
          enabled(true),
          visible(true),
          callback(callback),
          scene(scene)
{
}

bool TouchArea::containsPoint(int x, int y) const {
    if (!enabled) return false;

    return (x >= bounds.x &&
            x <= bounds.x + bounds.w &&
            y >= bounds.y &&
            y <= bounds.y + bounds.h);
}

void TouchArea::handleTouch(int x, int y, bool isPressed) {
    if (!enabled) return;

    // Check if the state has changed
    bool wasPressed = pressed;

    // Update pressed state
    if (isPressed && containsPoint(x, y)) {
        pressed = true;
    } else {
        pressed = false;
    }

    // Notify callback if state changed
    if (wasPressed != pressed && callback) {
        callback(pressed);
    }
}

void TouchArea::draw(SDL_Renderer* renderer) const {
    if (!visible) return;

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    // Set color based on pressed state
    const SDL_Color& fillColor = pressed ? pressedColor : normalColor;

    // Draw fill
    SDL_SetRenderDrawColor(renderer, fillColor.r, fillColor.g, fillColor.b, fillColor.a);
    SDL_RenderFillRect(renderer, &bounds);

    // Draw border
    SDL_SetRenderDrawColor(renderer, borderColor.r, borderColor.g, borderColor.b, borderColor.a);
    SDL_RenderDrawRect(renderer, &bounds);
}