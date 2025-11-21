#pragma once

#include "unordered_map"
#include "string"

#include "EnumSceneTag.h"
#include "AScene.h"

class SceneManager
{
private:
    std::unordered_map<SceneTag, AScene*> mapScene;
    AScene* pActiveScene = NULL;
    bool bLoading = false;
    SceneTag ESceneToLoad = SceneTag::NONE;
    SDL_Renderer* pRenderer;

public:
    void registerScene(AScene* pScene);
    void loadScene(SceneTag ETag);
    void unloadScene();
    void checkLoadScene();

public:
    bool isLoaded(SceneTag ETag);
    SceneTag getLoadedSceneTag() const;
    int getRegisteredSceneAmount() const;


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
