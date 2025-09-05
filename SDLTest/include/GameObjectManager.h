#pragma once

#include "EnumComponentType.h"
#include "GameObject.h"
//#include "GameStateManager.hpp"
#include <SDL3/SDL.h>
#include <unordered_map>

class GameObjectManager
{
private:
    std::unordered_map<std::string, GameObject*> mapGameObject;
    std::vector<GameObject*> vecGameObject;

public:
    void processInput(SDL_Event eEvent);
    void update();
    void draw(SDL_Window* pWindow);

    void addObject(GameObject* pGameObject);
    void deleteObject(GameObject* pGameObject);
    void deleteObjectByName(std::string strName);
    void deleteAllObjects();

    GameObject* findObjectByName(std::string strName);

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
