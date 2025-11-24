// ---------------------------------------------------------------------------
// Responsibilities: register scenes, schedule and perform loads/unloads.
// ---------------------------------------------------------------------------

#include "SceneManager.h"
#include "MetricsManager.h"

void SceneManager::registerScene(AScene* pScene)
{
    // O(1): Inserts into unordered map.
    this->mapScene[pScene->getTag()] = pScene;
}

void SceneManager::loadScene(SceneTag ETag)
{
    // O(1): Marks a scene to load, defers actual loading.
    if (this->ESceneToLoad != ETag)
    {
        this->bLoading = true;
        this->ESceneToLoad = ETag;
    }

    // start load timer now (actual load finishes in checkLoadScene) 
    MetricsManager::getInstance()->startLoadTimer();
}

// unloadScene: calls scene callbacks which may perform object/resource teardown
// Complexity: O(1) control-wise, but scene callbacks may be O(G) or worse
// depending on how scenes manage their contents.
void SceneManager::unloadScene()
{
    // O(1): Unloads current scene’s objects/resources.
    if(this->pActiveScene != NULL)
    {
        this->pActiveScene->onUnloadObjects();
        this->pActiveScene->onUnloadResources();
    }
}

// checkLoadScene: when bLoading true, perform unload, set active scene, and
// call onLoadResources/onLoadObjects. Complexity: O(1) control; hidden cost
// equals whatever the scene load callbacks perform (which can scale with
// number of assets, objects, textures, etc.). Large scene loads produce
// blocking spikes proportional to load_work.
void SceneManager::checkLoadScene()
{
    // O(1): Checks if loading is pending; executes load steps.
    // Could have hidden O(N) in object loading, depending on scene content.
    if(this->bLoading)
    {
        this->unloadScene();
        this->pActiveScene = this->mapScene[this->ESceneToLoad];

        this->pActiveScene->onLoadResources();
        this->pActiveScene->onLoadObjects();

        this->bLoading = false;

        // end load timer now (scene is fully loaded and ready) 
        MetricsManager::getInstance()->endLoadTimer();
    }
}

bool SceneManager::isLoaded(SceneTag ETag)
{
    return this->pActiveScene->getTag() == ETag; // O(1)
}

SceneTag SceneManager::getLoadedSceneTag() const
{
    return this->ESceneToLoad; // O(1)
}

int SceneManager::getRegisteredSceneAmount() const
{
    return this->mapScene.size(); // O(1) for map::size()
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
