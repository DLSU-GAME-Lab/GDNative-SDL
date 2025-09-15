#include "SceneManager.h"

void SceneManager::registerScene(AScene* pScene)
{
    this->mapScene[pScene->getTag()] = pScene;
}

void SceneManager::loadScene(SceneTag ETag)
{
    this->bLoading = true;
    this->ESceneToLoad = ETag;
    //create Loading AScene
}

void SceneManager::unloadScene()
{
    if(this->pActiveScene != NULL)
    {
        this->pActiveScene->onUnloadObjects();
        this->pActiveScene->onUnloadResources();
    }
}

void SceneManager::checkLoadScene()
{
    if(this->bLoading)
    {
        this->unloadScene();
        this->pActiveScene = this->mapScene[this->ESceneToLoad];

        this->pActiveScene->onLoadResources();
        this->pActiveScene->onLoadObjects();

        this->bLoading = false;
    }
}

bool SceneManager::isLoaded(SceneTag ETag)
{
    return this->pActiveScene->getTag() == ETag;
}

/* * * * * * * * * * * * * * * * * * * * * 
 *       SINGLETON-RELATED CONTENT       * 
 * * * * * * * * * * * * * * * * * * * * */
SceneManager* SceneManager::P_SHARED_INSTANCE = NULL;

void SceneManager::initialize()
{
    P_SHARED_INSTANCE = new SceneManager();
}

void SceneManager::destroy()
{
    delete P_SHARED_INSTANCE;
}

SceneManager* SceneManager::getInstance()
{
    return P_SHARED_INSTANCE;
}
/* * * * * * * * * * * * * * * * * * * * */
