#pragma once
#include "EnumSceneTag.h"
#include "Settings.h"
#include <SDL3/SDL.h>

enum class TransitionType {
    NONE,
    FADE
};

class SceneTransitionManager {
private:
    static SceneTransitionManager* P_SHARED_INSTANCE;

    bool inTransition = false;
    bool halfway = false;          // track fade midpoint
    float alpha = 0.0f;            // fade opacity (0-1)
    float speed = 0.02f;           // fade step per frame
    TransitionType type = TransitionType::NONE;
    SceneTag targetScene;

public:
    void requestTransition(SceneTag nextScene, TransitionType type = TransitionType::FADE);

    void update();
    void draw(SDL_Renderer* pRenderer);

    bool isTransitioning() const { return inTransition; }

    static void initialize();
    static void destroy();
    static SceneTransitionManager* getInstance();
};
