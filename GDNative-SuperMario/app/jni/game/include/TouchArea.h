// TouchArea.h
#ifndef GDNATIVE_SUPERMARIO_TOUCHAREA_H
#define GDNATIVE_SUPERMARIO_TOUCHAREA_H

#include "SDL.h"
#include <string>
#include <functional>

class TouchArea {
public:
    // Type definition for callback function
    using TouchCallback = std::function<void(bool)>;

    // Constructor
    TouchArea(const std::string& id, SDL_Rect bounds,
              TouchCallback callback = nullptr,
              const std::string& scene = "global");

    // Destructor
    ~TouchArea() = default;

    // Add a scene identifier
    std::string scene;

    // Check if point is inside this touch area
    bool containsPoint(int x, int y) const;

    // Handle touch events
    void handleTouch(int x, int y, bool isPressed);

    // Draw the touch area
    void draw(SDL_Renderer* renderer) const;

    // Getters and setters
    const std::string& getId() const { return id; }
    const SDL_Rect& getBounds() const { return bounds; }
    void setBounds(const SDL_Rect& newBounds) { bounds = newBounds; }

    bool isPressed() const { return pressed; }
    void setPressed(bool isPressed) { pressed = isPressed; }

    bool isEnabled() const { return enabled; }
    void setEnabled(bool isEnabled) { enabled = isEnabled; }

    bool isVisible() const { return visible; }
    void setVisible(bool isVisible) { visible = isVisible; }

    void setCallback(TouchCallback newCallback) { callback = newCallback; }

    const std::string& getScene() const { return scene; }
    void setScene(const std::string& newScene) { scene = newScene; }

    // Color settings
    SDL_Color normalColor{200, 200, 200, 120};   // Color when not pressed
    SDL_Color pressedColor{200, 200, 200, 180};  // Color when pressed
    SDL_Color borderColor{255, 255, 255, 200};   // Border color

private:
    std::string id;       // Unique identifier for this touch area
    SDL_Rect bounds;      // Position and size
    bool pressed;         // Is the touch area currently pressed?
    bool enabled;         // Is the touch area enabled for input?
    bool visible;         // Should the touch area be drawn?
    TouchCallback callback; // Function to call when touch state changes
};

#endif //GDNATIVE_SUPERMARIO_TOUCHAREA_H