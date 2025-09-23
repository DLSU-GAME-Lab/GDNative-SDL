#include "SceneTransitionManager.h"
#include "SceneManager.h"
#include <iostream>

SceneTransitionManager* SceneTransitionManager::P_SHARED_INSTANCE = nullptr;

void SceneTransitionManager::initialize() {
    if (P_SHARED_INSTANCE == nullptr) {
        P_SHARED_INSTANCE = new SceneTransitionManager();
        std::cout << "[SceneTransitionManager] Initialized." << std::endl;
    }
}

void SceneTransitionManager::destroy() {
    delete P_SHARED_INSTANCE;
    P_SHARED_INSTANCE = nullptr;
    std::cout << "[SceneTransitionManager] Destroyed." << std::endl;
}

SceneTransitionManager* SceneTransitionManager::getInstance() {
    return P_SHARED_INSTANCE;
}

void SceneTransitionManager::requestTransition(SceneTag nextScene, TransitionType type) {
    if (inTransition) {
        std::cout << "[SceneTransitionManager] Already transitioning." << std::endl;
        return;
    }

    this->targetScene = nextScene;
    this->type = type;
    this->alpha = 0.0f;
    this->speed = 0.02f;
    this->inTransition = true;
    this->halfway = false;

    std::cout << "[SceneTransitionManager] Transition started." << std::endl;
}

void SceneTransitionManager::update() {
    if (!inTransition) return;

    alpha += speed;

    // Hit midpoint (fully black)
    if (!halfway && alpha >= 1.0f) {
        halfway = true;

        std::cout << "[SceneTransitionManager] Midpoint reached, loading new scene..." << std::endl;
        SceneManager::getInstance()->loadScene(targetScene);
        SceneManager::getInstance()->checkLoadScene(); // Force-load immediately

        // Reverse fade direction
        speed = -speed;
    }

    // End transition
    if (halfway && alpha <= 0.0f) {
        inTransition = false;
        type = TransitionType::NONE;
        speed = -speed; // reset
        std::cout << "[SceneTransitionManager] Transition finished." << std::endl;
    }
}

void SceneTransitionManager::draw(SDL_Renderer* pRenderer) {
    if (!inTransition || type == TransitionType::NONE) return;

    SDL_SetRenderDrawBlendMode(pRenderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(pRenderer, 0, 0, 0, (Uint8)(alpha * 255));

    SDL_FRect fullscreen = { 0, 0, (float)gameWidth, (float)gameHeight };
    SDL_RenderFillRect(pRenderer, &fullscreen);

    SDL_SetRenderDrawBlendMode(pRenderer, SDL_BLENDMODE_NONE);
}
