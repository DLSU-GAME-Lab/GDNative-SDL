#pragma once

#include "Component.h"
#include <SDL3/SDL.h>
#include <vector>

class GameObject
{
protected:
    bool bEnabled;
    std::string strName;

    GameObject* pParent;
    std::vector<GameObject*> vecChildren;
    std::vector<Component*> vecComponent;

public:
    GameObject(std::string strName);
    virtual ~GameObject() = default;

public:
    virtual void initialize() = 0;
    virtual void processInput(SDL_Event eEvent);
    virtual void update();
    virtual void draw(SDL_Window* pWindow);

public:
    void attachChild(GameObject* pChild);
    void detachChild(GameObject* pChild);
    GameObject* findChildByName(std::string strName);

    void attachComponent(Component* pComponent);
    void detachComponent(Component* pComponent);
    Component* findComponentByName(std::string strName);
    std::vector<Component*> getComponents(ComponentType EType);
    std::vector<Component*> getComponentsRecursively(ComponentType EType, bool bInclusive = true);

public:
    bool isEnabled();
    void setEnabled(bool bEnabled);
    std::string getName();
    GameObject* getParent();
    void setParent(GameObject* pParent);
};
