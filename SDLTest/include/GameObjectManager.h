#pragma once

#include "EnumComponentType.h"
#include "AGameObject.h"
//#include "GameStateManager.hpp"
#include <SDL3/SDL.h>
#include <unordered_map>

class GameObjectManager
{
private:
    std::unordered_map<std::string, AGameObject*> mapGameObject;
    std::vector<AGameObject*> vecGameObject;

public:
    void processInput(SDL_Event* eEvent);
    void update();
    void draw(SDL_Window* pWindow);

    void addObject(AGameObject* pGameObject);
    void deleteObject(AGameObject* pGameObject);
    void deleteObjectByName(std::string strName);
    void deleteAllObjects();

    AGameObject* findObjectByName(std::string strName);
    std::vector<AGameObject*>& getAllObjects();

    /* * * * * * * * * * * * * * * * * * * * *
     *       SINGLETON-RELATED CONTENT       *
     * * * * * * * * * * * * * * * * * * * * */
private:
    static GameObjectManager* P_SHARED_INSTANCE;

private:
    GameObjectManager() {};
    GameObjectManager(const GameObjectManager&) {};
    GameObjectManager& operator = (const GameObjectManager&) {};

public:
    static void initialize();
    static void destroy();

    static GameObjectManager* getInstance();
    /* * * * * * * * * * * * * * * * * * * * */
};
