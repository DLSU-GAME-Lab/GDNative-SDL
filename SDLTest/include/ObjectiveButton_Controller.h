#pragma once
#include "AComponent.h"
#include "Vector2D.h"
#include "Text.h"

class AGameObject;
class ButtonInput;
class SpriteRenderer;
class AComponent;

class ObjectiveButton_Controller : public AComponent {
public:
    ObjectiveButton_Controller();
    virtual ~ObjectiveButton_Controller();

    void initialize();
    void perform() override;

    void toggle(); // programmatic toggle

    // animation settings
    float animDuration = 0.20f;     // seconds
    float collapsedScaleX = 0.20f;  // narrow arrow width
    float expandedScaleX = 1.0f;    // full button width

private:
    enum class State { Idle, Expanding, Collapsing };
    State state = State::Idle;

    AGameObject* ownerObj = nullptr;     // the GUIButton
    AGameObject* arrowObj = nullptr;     // child arrow (always shown)
    AGameObject* labelObj = nullptr;     // child label background (shown when expanded)
    ButtonInput* btnInput = nullptr;
    AGameObject* labelTextObj = nullptr; // child label text

    float elapsed = 0.0f;
    float startScaleX = 1.0f;
    float targetScaleX = 1.0f;

    float smoothstep(float t);
};
