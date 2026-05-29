#pragma once

#include "EnumComponentType.h"
#include "AGameObject.h"
#include <unordered_map>
#include <vector>
#include <string>

// ---------------------------------------------------------------------------
// GameObjectManager
// Owns and iterates over all AGameObject instances for input, update, draw.
// Deletion is deferred to cleanUpDeletedObjects() to avoid iterator
// invalidation during traversal.
// ---------------------------------------------------------------------------
class GameObjectManager
{
public:
    // --- Per-frame lifecycle ---------------------------------------------------
    void processInput(SDL_Event* pEvent);
    void update(float fDeltaTime);
    void draw(SDL_Renderer* pRenderer);

    // Flush the deferred-deletion queue. Call once per frame after update/draw.
    void cleanUpDeletedObjects();

    // --- Object management ----------------------------------------------------

    // Takes ownership of pGameObject; calls pGameObject->initialize().
    // Asserts that pGameObject is non-null.
    void addObject(AGameObject* pGameObject);

    // Schedules pGameObject for deletion at the next cleanUpDeletedObjects().
    // Safe to call during traversal. No-op if already scheduled.
    void deleteObject(AGameObject* pGameObject);

    void deleteObjectByName(const std::string& strName);

    // Immediately frees all objects and clears internal containers.
    // Do NOT call during a processInput / update / draw traversal.
    void deleteAllObjects();

    // --- Queries --------------------------------------------------------------

    // Returns nullptr (not a side-effecting operator[]) when name is absent.
    AGameObject* findObjectByName(const std::string& strName) const;

    // Renames an existing object; no-op (with error log) if strName not found.
    void setObjectName(const std::string& strName, const std::string& strNewName);

    // O(1) — callers that iterate will pay O(G).
    std::vector<AGameObject*>& getAllObjects();

    /* * * * * * * * * * * * * * * * * * * * *
     *       SINGLETON-RELATED CONTENT       *
     * * * * * * * * * * * * * * * * * * * * */
public:
    static void initialize();
    static void destroy();
    static GameObjectManager* getInstance();

private:
    static GameObjectManager* P_SHARED_INSTANCE;

    GameObjectManager() = default;
    ~GameObjectManager();

    // Non-copyable, non-movable.
    GameObjectManager(const GameObjectManager&) = delete;
    GameObjectManager& operator=(const GameObjectManager&) = delete;
    GameObjectManager(GameObjectManager&&) = delete;
    GameObjectManager& operator=(GameObjectManager&&) = delete;
    /* * * * * * * * * * * * * * * * * * * * */

private:
    std::unordered_map<std::string, AGameObject*> mapGameObject;
    std::vector<AGameObject*>                     vecGameObject;
    std::vector<AGameObject*>                     vecPendingDeletion;
};
