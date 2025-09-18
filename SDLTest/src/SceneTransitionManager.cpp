#include "SceneTransitionManager.h"
#include "SceneManager.h"
#include <SDL3/SDL.h>

SceneTransitionManager* SceneTransitionManager::P_SHARED_INSTANCE = nullptr;

void SceneTransitionManager::initialize() {
    if (P_SHARED_INSTANCE == nullptr) {
        P_SHARED_INSTANCE = new SceneTransitionManager();
        std::cout << "[SceneTransitionManager] Initialized." << std::endl;
    }
}

void SceneTransitionManager::destroy() {
    if (P_SHARED_INSTANCE != nullptr) {
        delete P_SHARED_INSTANCE;
        P_SHARED_INSTANCE = nullptr;
        std::cout << "[SceneTransitionManager] Destroyed." << std::endl;
    }
}

SceneTransitionManager* SceneTransitionManager::getInstance() {
    if (!P_SHARED_INSTANCE) {
        std::cerr << "[SceneTransitionManager ERROR] getInstance() called before initialize!" << std::endl;
    }
    return P_SHARED_INSTANCE;
}

void SceneTransitionManager::requestTransition(SceneTag nextScene, TransitionType type) {
    if (inTransition) {
        std::cout << "[SceneTransitionManager] Transition already in progress, ignoring new request." << std::endl;
        return;
    }

    std::cout << "[SceneTransitionManager] Starting transition to scene: "
        << (int)nextScene << " with type=" << (int)type << std::endl;

    this->targetScene = nextScene;
    this->type = type;
    this->alpha = 0.0f;
    this->speed = 0.02f;
    this->inTransition = true;
}

void SceneTransitionManager::update() {
    if (!inTransition) return;

    alpha += speed;
    std::cout << "[SceneTransitionManager] Updating alpha=" << alpha << " speed=" << speed << std::endl;

    if (alpha >= 1.0f) {
        std::cout << "[SceneTransitionManager] Halfway reached, loading new scene..." << std::endl;
        SceneManager::getInstance()->loadScene(targetScene);

        // start fading back in
        speed = -speed;
    }

    if (alpha <= 0.0f && speed < 0) {
        std::cout << "[SceneTransitionManager] Transition finished." << std::endl;
        inTransition = false;
        type = TransitionType::NONE;
        speed = -speed; // reset
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
