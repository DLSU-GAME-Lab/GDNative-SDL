#include "SceneManager.h"
#include <iostream>

void SceneManager::registerScene(SceneTag tag, std::unique_ptr<AScene> scene)
{
    mapScene[tag] = std::move(scene);
}

void SceneManager::loadScene(SceneTag ETag)
{
    if (mapScene.find(ETag) == mapScene.end()) {
        std::cerr << "[ERROR] Scene with tag " << static_cast<int>(ETag) << " not registered!" << std::endl;
        return;
    }

    if (ETag == ECurrentScene && pActiveScene != nullptr) {
        return; // already on this scene
    }

    ENextScene = ETag;
    pNextScene = mapScene[ETag].get();
    transitionState = TransitionState::TRANSITION_OUT;
    transitionProgress = 0.0f;
}

void SceneManager::unloadCurrentScene()
{
    if(this->pActiveScene != NULL)
    {
        if (pActiveScene != nullptr)
        {
            pActiveScene->onUnloadObjects();
            pActiveScene->onUnloadResources();
        }
    }
}

//This shouldn't be here. Scene Manager only manages scene objects and resources. Move somewhere else.
void SceneManager::update(float deltaTime)
{
    // handle scene transitions and updates
    if (transitionState == TransitionState::NONE) {
        if (pActiveScene) {
            pActiveScene->update(deltaTime);
        }
        return;
    }

    // handle scene transitions
    transitionProgress += deltaTime;

    switch (transitionState) {
    case TransitionState::TRANSITION_OUT:
        if (transitionProgress >= transitionDuration) {
            // transition out complete, now unload and load new scene
            unloadCurrentScene();
            pActiveScene = pNextScene;
            ECurrentScene = ENextScene;
            pActiveScene->onLoadResources();
            pActiveScene->onLoadObjects();
            transitionState = TransitionState::TRANSITION_IN;
            transitionProgress = 0.0f;
        }
        break;

    case TransitionState::TRANSITION_IN:
        if (transitionProgress >= transitionDuration) {
            transitionState = TransitionState::COMPLETE;
            transitionProgress = 0.0f;
        }
        break;

    case TransitionState::COMPLETE:
        transitionState = TransitionState::NONE;
        break;

    default:
        break;
    }

    // update current scene even during transitions
    if (pActiveScene) {
        pActiveScene->update(deltaTime);
    }
}

//Aslo should be moved.
void SceneManager::render(SDL_Renderer* pRenderer)
{
    if (!pActiveScene) return;

    // transition effects (fade, slide, etc.)
    float alpha = 1.0f;

    if (transitionState == TransitionState::TRANSITION_OUT) {
        alpha = 1.0f - (transitionProgress / transitionDuration);
    }
    else if (transitionState == TransitionState::TRANSITION_IN) {
        alpha = transitionProgress / transitionDuration;
    }

    // render the scene
    pActiveScene->render(pRenderer);

    // set renderer alpha modulation for fade effect
    SDL_SetRenderDrawBlendMode(pRenderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(pRenderer, 0, 0, 0, static_cast<Uint8>(255 * (1.0f - alpha)));
    SDL_RenderFillRect(pRenderer, nullptr);
}

void SceneManager::checkLoadScene()
{
    // now handled in the update method
}

bool SceneManager::isLoaded(SceneTag ETag) const
{
    return pActiveScene && pActiveScene->getTag() == ETag;
}

/* * * * * * * * * * * * * * * * * * * * * 
 *       SINGLETON-RELATED CONTENT       * 
 * * * * * * * * * * * * * * * * * * * * */
SceneManager* SceneManager::P_SHARED_INSTANCE = nullptr;

void SceneManager::initialize()
{
    if (P_SHARED_INSTANCE == nullptr) {
        P_SHARED_INSTANCE = new SceneManager();
    }
}

void SceneManager::destroy()
{
    delete P_SHARED_INSTANCE;
    P_SHARED_INSTANCE = nullptr;
}

SceneManager* SceneManager::getInstance()
{
    return P_SHARED_INSTANCE;
}
/* * * * * * * * * * * * * * * * * * * * */
