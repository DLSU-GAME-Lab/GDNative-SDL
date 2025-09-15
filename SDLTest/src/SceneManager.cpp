#include "SceneManager.h"

void SceneManager::registerScene(AScene* pScene)
{
    this->mapScene[pScene->getTag()] = pScene;
}

void SceneManager::loadScene(SceneTag ETag)
{
    if (this->ESceneToLoad != ETag)
    {
        this->bLoading = true;
        this->ESceneToLoad = ETag;
    }
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

SceneTag SceneManager::getLoadedSceneTag() const
{
    return this->ESceneToLoad;
}

int SceneManager::getRegisteredSceneAmount() const
{
    return this->mapScene.size();
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
