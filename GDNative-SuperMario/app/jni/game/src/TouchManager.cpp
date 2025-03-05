//
// Created by Rylan on 3/5/2025.
//
// TouchManager.cpp
#include "TouchManager.h"
#include "CFG.h"

// Define the static string constants
const std::string TouchControlID::DPAD_UP = "dpad_up";
const std::string TouchControlID::DPAD_DOWN = "dpad_down";
const std::string TouchControlID::DPAD_LEFT = "dpad_left";
const std::string TouchControlID::DPAD_RIGHT = "dpad_right";
const std::string TouchControlID::BUTTON_A = "button_a";
const std::string TouchControlID::BUTTON_B = "button_b";
const std::string TouchControlID::PAUSE = "pause";

TouchManager* TouchManager::getInstance() {
    static TouchManager instance;  // Single instance created once
    return &instance;              // Return pointer to the instance
}

void TouchManager::initialize(int screenWidth, int screenHeight) {
    this->screenWidth = screenWidth;
    this->screenHeight = screenHeight;

    // Clear existing touch areas
    touchAreas.clear();
    drawOrder.clear();

    // D-pad controls
    int dpadSize = 50;
    int dpadSpacing = 150;
    int dpadY = CCFG::GAME_HEIGHT - 140;

    // Create D-pad controls
    addTouchArea(TouchControlID::DPAD_LEFT,
                 {20, dpadY, dpadSize, dpadSize});

    addTouchArea(TouchControlID::DPAD_RIGHT,
                 {20 + dpadSpacing, dpadY, dpadSize, dpadSize});

    addTouchArea(TouchControlID::DPAD_UP,
                 {20 + dpadSpacing/2, dpadY - dpadSize, dpadSize, dpadSize});

    addTouchArea(TouchControlID::DPAD_DOWN,
                 {20 + dpadSpacing/2, dpadY + dpadSize, dpadSize, dpadSize});

    // Action buttons
    int buttonSize = 50;
    int buttonY = CCFG::GAME_HEIGHT - 150;

    // Create A and B buttons
    addTouchArea(TouchControlID::BUTTON_A,
                 {CCFG::GAME_WIDTH - 80, buttonY, buttonSize, buttonSize});

    // Set A button color to red
    if (TouchArea* aButton = getTouchArea(TouchControlID::BUTTON_A)) {
        aButton->normalColor = {255, 50, 50, 120};
        aButton->pressedColor = {255, 50, 50, 180};
    }

    addTouchArea(TouchControlID::BUTTON_B,
                 {CCFG::GAME_WIDTH - 150, buttonY, buttonSize, buttonSize});

    // Set B button color to blue
    if (TouchArea* bButton = getTouchArea(TouchControlID::BUTTON_B)) {
        bButton->normalColor = {50, 50, 255, 120};
        bButton->pressedColor = {50, 50, 255, 180};
    }

    // Pause button
    addTouchArea(TouchControlID::PAUSE,
                 {CCFG::GAME_WIDTH - 50, 20, 40, 40});
}

void TouchManager::addTouchArea(const std::string& id, SDL_Rect bounds, TouchArea::TouchCallback callback) {
    // Create the touch area
    auto touchArea = std::make_unique<TouchArea>(id, bounds, callback);

    // Add to map and draw order
    touchAreas[id] = std::move(touchArea);
    drawOrder.push_back(id);
}

void TouchManager::removeTouchArea(const std::string& id) {
    // Remove from map
    touchAreas.erase(id);

    // Remove from draw order
    auto it = std::find(drawOrder.begin(), drawOrder.end(), id);
    if (it != drawOrder.end()) {
        drawOrder.erase(it);
    }
}

void TouchManager::setAllTouchAreasOpacity(float opacity) {
    // Ensure opacity is within valid range [0.0, 1.0]
    opacity = std::max(0.0f, std::min(1.0f, opacity));

    // Calculate alpha value (0-255)
    Uint8 alpha = static_cast<Uint8>(255 * opacity);

    // Update each touch area's colors
    for (auto& [id, touchArea] : touchAreas) {
        // Update normal color alpha
        touchArea->normalColor.a = alpha;

        // Update pressed color alpha
        touchArea->pressedColor.a = alpha;

        // Update border color alpha
        touchArea->borderColor.a = alpha;
    }
}

TouchArea* TouchManager::getTouchArea(const std::string& id) {
    auto it = touchAreas.find(id);
    if (it != touchAreas.end()) {
        return it->second.get();
    }
    return nullptr;
}

bool TouchManager::hasTouchArea(const std::string& id) const {
    return touchAreas.find(id) != touchAreas.end();
}

void TouchManager::handleTouchEvent(int x, int y, bool isPressed) {
    // Process all touch areas
    for (auto& [id, touchArea] : touchAreas) {
        touchArea->handleTouch(x, y, isPressed);
    }
}

void TouchManager::drawTouchAreas(SDL_Renderer* renderer) {
    // Draw in the order specified by drawOrder
    for (const auto& id : drawOrder) {
        auto it = touchAreas.find(id);
        if (it != touchAreas.end()) {
            it->second->draw(renderer);
        }
    }
}

void TouchManager::setTouchAreaEnabled(const std::string& id, bool enabled) {
    if (TouchArea* area = getTouchArea(id)) {
        area->setEnabled(enabled);
    }
}

void TouchManager::setTouchAreaVisible(const std::string& id, bool visible) {
    if (TouchArea* area = getTouchArea(id)) {
        area->setVisible(visible);
    }
}

void TouchManager::setAllTouchAreasEnabled(bool enabled) {
    for (auto& [id, touchArea] : touchAreas) {
        touchArea->setEnabled(enabled);
    }
}

void TouchManager::setAllTouchAreasVisible(bool visible) {
    for (auto& [id, touchArea] : touchAreas) {
        touchArea->setVisible(visible);
    }
}

void TouchManager::setTouchAreaColors(const std::string& id,
                                      const SDL_Color& normalColor,
                                      const SDL_Color& pressedColor,
                                      const SDL_Color& borderColor) {
    if (TouchArea* area = getTouchArea(id)) {
        area->normalColor = normalColor;
        area->pressedColor = pressedColor;
        area->borderColor = borderColor;
    }
}

void TouchManager::updateTouchAreaBounds(const std::string& id, const SDL_Rect& bounds) {
    if (TouchArea* area = getTouchArea(id)) {
        area->setBounds(bounds);
    }
}
