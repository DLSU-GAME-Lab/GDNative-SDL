//
// Created by Rylan on 3/5/2025.
//

#ifndef GDNATIVE_SUPERMARIO_TOUCHMANAGER_H
#define GDNATIVE_SUPERMARIO_TOUCHMANAGER_H

#include "TouchArea.h"
#include <unordered_map>
#include <memory>
#include <vector>
#include <string>

// Predefined touch control IDs for game
struct TouchControlID {
    // String constants defined in TouchManager.cpp
    static const std::string DPAD_UP;
    static const std::string DPAD_DOWN;
    static const std::string DPAD_LEFT;
    static const std::string DPAD_RIGHT;
    static const std::string BUTTON_A;
    static const std::string BUTTON_B;
    static const std::string PAUSE;
};

class TouchManager {
public:
    // Get singleton instance - returns pointer to singleton
    static TouchManager* getInstance();

    // Delete copy and move constructors and assignment operators
    TouchManager(const TouchManager&) = delete;
    TouchManager& operator=(const TouchManager&) = delete;
    TouchManager(TouchManager&&) = delete;
    TouchManager& operator=(TouchManager&&) = delete;

    // Initialize touch controls for specific screen dimensions
    void initialize(int screenWidth, int screenHeight);

    // Add a touch area
    void addTouchArea(const std::string& id, SDL_Rect bounds, TouchArea::TouchCallback callback = nullptr);

    // Remove a touch area
    void removeTouchArea(const std::string& id);

    void setAllTouchAreasOpacity(float opacity);

    // Get a touch area
    TouchArea* getTouchArea(const std::string& id);

    // Check if a touch area exists
    bool hasTouchArea(const std::string& id) const;

    // Handle touch events (mouse or finger)
    void handleTouchEvent(int x, int y, bool isPressed);

    // Draw all touch areas
    void drawTouchAreas(SDL_Renderer* renderer);

    // Enable/disable a specific touch area
    void setTouchAreaEnabled(const std::string& id, bool enabled);

    // Set visibility for a specific touch area
    void setTouchAreaVisible(const std::string& id, bool visible);

    // Enable/disable all touch areas
    void setAllTouchAreasEnabled(bool enabled);

    // Set visibility for all touch areas
    void setAllTouchAreasVisible(bool visible);

    // Set custom color for a touch area
    void setTouchAreaColors(const std::string& id,
                            const SDL_Color& normalColor,
                            const SDL_Color& pressedColor,
                            const SDL_Color& borderColor);

    // Update touch area bounds (useful for screen resizing)
    void updateTouchAreaBounds(const std::string& id, const SDL_Rect& bounds);

private:
    // Private constructor for singleton
    TouchManager() = default;

    // Container for touch areas
    std::unordered_map<std::string, std::unique_ptr<TouchArea>> touchAreas;

    // Ordered list of touch area IDs for drawing in correct order
    std::vector<std::string> drawOrder;

    // Cached screen dimensions
    int screenWidth = 0;
    int screenHeight = 0;
};

#endif //GDNATIVE_SUPERMARIO_TOUCHMANAGER_H
