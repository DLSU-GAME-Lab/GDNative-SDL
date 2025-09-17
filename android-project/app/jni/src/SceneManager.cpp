#include "SceneManager.h"
#include <iostream>
#include "spdlog/spdlog.h"
#include "spdlog/sinks/android_sink.h"

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

    // ANDROID STUDIO DEBUGS
    // if already active, nothing to do
    if (transitionState != TransitionState::NONE) {
        spdlog::debug("Scene transition already in progress, ignoring load request");
        return; // already in transition
    }

    if (ETag == ECurrentScene && pActiveScene != nullptr) {
        spdlog::debug("Scene {} is already active", static_cast<int>(ETag));
        return; // already on this scene
    }

    ENextScene = ETag;
    pNextScene = mapScene[ETag].get();
    transitionState = TransitionState::TRANSITION_OUT;
    transitionProgress = 0.0f;
}

void SceneManager::unloadCurrentScene()
{
    if (pActiveScene != nullptr) {
        spdlog::debug("Unloading current scene: {}", static_cast<int>(ECurrentScene));
        pActiveScene->onUnloadObjects();
        pActiveScene->onUnloadResources();
        // DON'T set pActiveScene = nullptr here!
        // The transition logic will handle assigning the new scene
    }
}

//This shouldn't be here. Scene Manager only manages scene objects and resources. Move somewhere else.
void SceneManager::update(float deltaTime)
{
    spdlog::info("SceneManager::update() deltaTime = {}, progress = {}, duration = {}, state = {}",
                  deltaTime, transitionProgress, transitionDuration, static_cast<int>(transitionState));

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
        spdlog::debug("TRANSITION_OUT - Progress: {:.2f}/{:.2f}", transitionProgress, transitionDuration);
        if (transitionProgress >= transitionDuration) {
            // transition out complete, now unload and load new scene
            spdlog::info("TRANSITION_OUT complete, switching to new scene");
            unloadCurrentScene();
            pActiveScene = pNextScene;
            ECurrentScene = ENextScene;
            spdlog::info("New scene pointer: {}, Tag: {}", (void*)pActiveScene, static_cast<int>(ECurrentScene));

            if (pActiveScene) {
                spdlog::info("Loading scene resources...");
                pActiveScene->onLoadResources();
                spdlog::info("Loading scene objects...");
                pActiveScene->onLoadObjects();
                spdlog::info("New scene resources loaded");
            } else {
                spdlog::error("pActiveScene is NULL after assignment!");
            }
            transitionState = TransitionState::TRANSITION_IN;
            transitionProgress = 0.0f;
            spdlog::info("New scene loaded, starting TRANSITION_IN");
        }
        break;

    case TransitionState::TRANSITION_IN:
        spdlog::debug("TRANSITION_IN - Progress: {:.2f}/{:.2f}", transitionProgress, transitionDuration);
        if (transitionProgress >= transitionDuration) {
            transitionState = TransitionState::COMPLETE;
            transitionProgress = 0.0f;
            spdlog::info("TRANSITION_IN complete");
        }
        break;

    case TransitionState::COMPLETE:
        spdlog::debug("COMPLETE - Finalizing transition");
        transitionState = TransitionState::NONE;
        spdlog::info("Scene transition complete");
        break;

    default:
        spdlog::warn("Unknown transition state: {}", static_cast<int>(transitionState));
        break;
    }

    // update current scene even during transitions
    if (pActiveScene) {
        // if we are in TRANSITION_OUT, and the current scene is being unloaded, skip its update
        if (!(transitionState == TransitionState::TRANSITION_OUT && pActiveScene == mapScene[ECurrentScene].get())) {
            pActiveScene->update(deltaTime);
        }
    }
}

//Also should be moved.
void SceneManager::render(SDL_Renderer* pRenderer)
{
    if (!pActiveScene) {
        spdlog::warn("No active scene to render");
        return;
    }

    // transition effects (fade, slide, etc.)
    float alpha = 1.0f;

    if (transitionState == TransitionState::TRANSITION_OUT) {
        alpha = 1.0f - (transitionProgress / transitionDuration);
        spdlog::debug("Rendering TRANSITION_OUT with alpha: {:.2f}", alpha);
    }
    else if (transitionState == TransitionState::TRANSITION_IN) {
        alpha = transitionProgress / transitionDuration;
        spdlog::debug("Rendering TRANSITION_IN with alpha: {:.2f}", alpha);
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
