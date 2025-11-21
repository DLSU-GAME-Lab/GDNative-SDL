#include "ObjectiveButton_Controller.h"
#include "AGameObject.h"
#include "GameObjectManager.h"
#include "ButtonInput.h"
#include "SpriteRenderer.h"
#include <iostream>

ObjectiveButton_Controller::ObjectiveButton_Controller()
    : AComponent("ObjectiveButtonController", ComponentType::SCRIPT)
{
}

ObjectiveButton_Controller::~ObjectiveButton_Controller() {}

void ObjectiveButton_Controller::initialize()
{
    ownerObj = this->getOwner(); 
    if (!ownerObj) return;

    // find the ButtonInput (attached to owner or its children)
    auto vecAll = ownerObj->getComponentsRecursively(ComponentType::INPUT, true);
    for (AComponent* c : vecAll) {
        ButtonInput* bi = dynamic_cast<ButtonInput*>(c);
        if (bi) { btnInput = bi; break; }
    }

    // find child objects by name patterns 
    arrowObj = ownerObj->findChildByName("ObjectiveArrow");   
    labelObj = ownerObj->findChildByName("ObjectiveLabelBG");
    labelTextObj = ownerObj->findChildByName("ObjectiveLabelText");

    // arrow is visible, label hidden initially (collapsed)
    if (arrowObj) arrowObj->setEnabled(true);
    if (labelObj) labelObj->setEnabled(false);
    if (labelTextObj) labelTextObj->setEnabled(false);

    // ensure initial collapsed/expanded state based on scale
    startScaleX = ownerObj->getScale().x;
    expandedScaleX = startScaleX; // align expanded to current owner scale

    // default start as expanded (you can set collapsed initially)
    state = State::Idle;
    elapsed = 0.0f;
}

float ObjectiveButton_Controller::smoothstep(float t)
{
    // smoother interpolation
    if (t <= 0.0f) return 0.0f;
    if (t >= 1.0f) return 1.0f;
    return t * t * (3.0f - 2.0f * t);
}

void ObjectiveButton_Controller::toggle()
{   
    // immediate toggle request (from code)
    if (state != State::Idle) return; // ignore while animating
    if (!ownerObj) return;

    float curX = ownerObj->getScale().x;
    
    // treat near-zero as collapsed
    if (curX > (collapsedScaleX + 0.01f)) {
        // currently expanded -> collapse
        state = State::Collapsing;
        startScaleX = curX;
        targetScaleX = collapsedScaleX;
    }
    else {
        // currently collapsed -> expand
        state = State::Expanding;
        startScaleX = curX;
        targetScaleX = expandedScaleX;
        if (labelObj) labelObj->setEnabled(true); // beginning of expand: make label visible
        if (labelTextObj) labelTextObj->setEnabled(true);
    }
    elapsed = 0.0f;
}

void ObjectiveButton_Controller::perform()
{
    // catch missing owner
    if (!ownerObj) ownerObj = this->getOwner();
    if (!ownerObj) return;
    
    // lazy-find missing references in case initialize didn't run or order was different
    if (!btnInput) {
        auto vecAll = ownerObj->getComponentsRecursively(ComponentType::INPUT, true);
        for (AComponent* c : vecAll) {
            ButtonInput* bi = dynamic_cast<ButtonInput*>(c);
            if (bi) { btnInput = bi; break; }
        }
    }
    if (!arrowObj) arrowObj = ownerObj->findChildByName("ObjectiveArrow");
    if (!labelObj) labelObj = ownerObj->findChildByName("ObjectiveLabelBG");
    if (!labelTextObj) labelTextObj = ownerObj->findChildByName("ObjectiveLabelText");

    // check for button click
    if (btnInput && btnInput->getClicked()) {
        toggle();
        btnInput->setClicked(false);
    }

    if (state == State::Idle) return;

    float dt = this->fDeltaTime;
    elapsed += dt;
    float t = elapsed / std::max(0.0001f, animDuration);
    float s = smoothstep(std::min(1.0f, t));
    float newX = startScaleX + (targetScaleX - startScaleX) * s;

    Vector2D sc = ownerObj->getScale();
    sc.x = newX;
    ownerObj->setScale(sc);

    // optionally move arrow/label relative positions if needed:
    // if label is anchored to the right side, adjust its local pos so it slides with button.

    if (t >= 1.0f) {
        // end animation
        if (state == State::Collapsing) {
            if (labelObj) labelObj->setEnabled(false);
        }
        state = State::Idle;
    }
}