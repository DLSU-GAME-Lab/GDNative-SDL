// ---------------------------------------------------------------------------
// Responsibilities: manage and iterate over game objects for input, update, draw.
// ---------------------------------------------------------------------------

#include "GameObjectManager.h"
#include "Collider.h"
#include <iostream>

// processInput: forwards a single SDL_Event to every enabled object.
// Complexity: O(G) per call.
// Scaling note: If there are E events this frame and processInput is called
// once per event, total input-processing work per frame is O(E * G). As G or
// E increase, this term can dominate frame cost.
void GameObjectManager::processInput(SDL_Event* eEvent)
{
    for(AGameObject* pGameObject : this->vecGameObject)
    {
        if(pGameObject->getEnabled())
            pGameObject->processInput(eEvent);
    }
}

// update: call each object's update(fDeltaTime)
// Complexity: O(G) per call when each object's update is constant-time.
// Scaling note: If object updates involve pairwise checks (e.g., collisions
// by brute-force), the inside of update may be O(G^2) leading to quadratic
// scaling as G increases.
void GameObjectManager::update(float fDeltaTime)
{
    for(AGameObject* pGameObject : this->vecGameObject)
    {
        if(pGameObject->getEnabled())
            pGameObject->update(fDeltaTime);
    }
}

// draw: call each object's draw(pRenderer)
// Complexity: O(R) per call where R is number of renderable/enabled objects.
// Scaling note: As R grows, GPU draw calls and driver overhead increase; even
// if algorithmic complexity is linear, wall-time may grow steeply with R.
void GameObjectManager::draw(SDL_Renderer* pRenderer)
{
    for(AGameObject* pGameObject : this->vecGameObject)
    {
        if(pGameObject->getEnabled())
            pGameObject->draw(pRenderer);
    }
}

// addObject: push_back to vector and add to map by name
// Complexity: amortized O(1) for insertion; map insertion average-case O(1)
// (unordered_map). As object count G increases, memory and traversal costs
// later scale with G.
void GameObjectManager::addObject(AGameObject* pGameObject)
{
    this->vecGameObject.push_back(pGameObject);
    this->mapGameObject[pGameObject->getName()] = pGameObject;
    pGameObject->initialize();
}

// deleteObject: find by scanning vector, erase, and delete
// Complexity: O(G) for search + O(G) for erase shifting => O(G).
// Scaling note: repeated per-object deletion across G elements (e.g. scene
// teardown) can accumulate to O(G^2) total work if each deletion performs a
// linear search or causes shifting.
void GameObjectManager::deleteObject(AGameObject* pGameObject)
{
    std::string strName = pGameObject->getName();
    int nIndex = -1;

    for(int i = 0; i < this->vecGameObject.size() && nIndex == -1; i++)
    {
        if(this->vecGameObject[i] == pGameObject)
            nIndex = i;
    }

    if(nIndex != -1) {
        this->mapGameObject.erase(this->vecGameObject[nIndex]->getName());
        this->vecGameObject.erase(this->vecGameObject.begin() + nIndex);
        delete pGameObject;
    }
}

// deleteObjectByName: name lookup (map) then delete
// Complexity: average-case O(1) for map lookup + O(G) for deletion step.
void GameObjectManager::deleteObjectByName(std::string strName)
{
    AGameObject* pGameObject = this->findObjectByName(strName);
    if(pGameObject != NULL)
        this->deleteObject(pGameObject);
}

// deleteAllObjects: current implementation iterates and deletes each object
// Complexity: depends on deleteObject implementation. If deleteObject performs
// a linear search each call, deleteAllObjects accumulates to O(G^2).
// Scaling note: large G during teardown can produce long blocking spikes.
void GameObjectManager::deleteAllObjects()
{
    // Iterate once: erase from map and delete each pointer.
    for (int i = 0; i < this->vecGameObject.size(); i++)
        delete this->vecGameObject[i];

    // Clear containers, O(1) operations relative to content
    this->vecGameObject.clear();
    this->mapGameObject.clear();
}
void GameObjectManager::sortObjectToEnd(AGameObject* pGameObject)
{
    int nIndex = -1, nIndex1 = this->vecGameObject.size() - 1;
    AGameObject* pHolder;
    for (int i = 0; i < this->vecGameObject.size() && nIndex == -1; i++)
    {
        if (vecGameObject[i]->getName() == pGameObject->getName())
        {
            nIndex = i;
        }
    }
    pHolder = this->vecGameObject[nIndex1];
    this->vecGameObject[nIndex1] = this->vecGameObject[nIndex];
    this->vecGameObject[nIndex] = pHolder;
}
// findObjectByName: map lookup
// Complexity: average-case O(1) (unordered_map) or O(log G) (ordered map).
// Note: using operator[] for lookup has side-effects (insertion) which may
// affect behavior as G grows
AGameObject* GameObjectManager::findObjectByName(std::string strName)
{
    if(this->mapGameObject[strName] != NULL)
        return this->mapGameObject[strName];

    else {
        std::cout << "[ERROR] : Object [" << strName << "] NOT found." << std::endl;
        return NULL;
    }
}

// getAllObjects: O(1) to return reference; iterating callers will pay O(G).
std::vector<AGameObject*>& GameObjectManager::getAllObjects()
{
    return this->vecGameObject;
}

/* * * * * * * * * * * * * * * * * * * * *
 *       SINGLETON-RELATED CONTENT       *
 * * * * * * * * * * * * * * * * * * * * */
GameObjectManager* GameObjectManager::P_SHARED_INSTANCE = NULL;

void GameObjectManager::initialize()
{
    P_SHARED_INSTANCE = new GameObjectManager();
}

void GameObjectManager::destroy()
{
    delete P_SHARED_INSTANCE;
}

GameObjectManager* GameObjectManager::getInstance()
{
    return P_SHARED_INSTANCE;
}
/* * * * * * * * * * * * * * * * * * * * */
