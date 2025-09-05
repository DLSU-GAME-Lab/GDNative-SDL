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
    SceneTag ESceneToLoad = SceneTag::TITLE_SCENE;

public:
    void registerScene(AScene* pScene);
    void loadScene(SceneTag ETag);
    void unloadScene();
    void checkLoadScene();

public:
    bool isLoaded(SceneTag ETag);

    /* * * * * * * * * * * * * * * * * * * * *
     *       SINGLETON-RELATED CONTENT       *
     * * * * * * * * * * * * * * * * * * * * */
private:
    static SceneManager* P_SHARED_INSTANCE;

private:
    SceneManager() {};
    SceneManager(const SceneManager&) {};
    SceneManager& operator=(const SceneManager&) {};

    static void initialize();
    static void destroy();

public:
    static SceneManager* getInstance();
    /* * * * * * * * * * * * * * * * * * * * */
};
