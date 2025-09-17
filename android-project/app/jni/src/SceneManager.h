#pragma once

#include "unordered_map"
#include "string"
#include "memory"

#include "EnumSceneTag.h"
#include "AScene.h"

class SceneManager
{
private:
    std::unordered_map<SceneTag, std::unique_ptr<AScene>> mapScene;
    AScene* pActiveScene = NULL;
    AScene* pNextScene = nullptr;
    //bool bLoading = false;
    //SceneTag ESceneToLoad = SceneTag::TITLE_SCENE;
    SceneTag ECurrentScene = SceneTag::TITLE_SCENE;
    SceneTag ENextScene = SceneTag::TITLE_SCENE;

    enum class TransitionState {
        NONE = 0,
        LOADING = 1,
        TRANSITION_OUT = 2,
        TRANSITION_IN = 3,
        COMPLETE = 4
    };

    TransitionState transitionState = TransitionState::NONE;
    float transitionProgress = 0.0f; 
    float transitionDuration = 0.5f; // seconds

public:
    void registerScene(SceneTag tag, std::unique_ptr<AScene> scene);
    void loadScene(SceneTag ETag);
    void unloadCurrentScene();
    void update(float deltaTime);
    void render(SDL_Renderer* pRenderer);
    void checkLoadScene();

    bool isInTransition() const { return transitionState != TransitionState::NONE; }
    bool isLoaded(SceneTag ETag) const;

    /* * * * * * * * * * * * * * * * * * * * *
     *       SINGLETON-RELATED CONTENT       *
     * * * * * * * * * * * * * * * * * * * * */
private:
    static SceneManager* P_SHARED_INSTANCE;

private:
    SceneManager() {};
    SceneManager(const SceneManager&) {};
    SceneManager& operator=(const SceneManager&) {};

public:
    static void initialize();
    static void destroy();

    static SceneManager* getInstance();
    /* * * * * * * * * * * * * * * * * * * * */
};
