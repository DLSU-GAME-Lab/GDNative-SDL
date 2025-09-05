#pragma once

#include "AComponent.h"
#include <SDL3/SDL.h>
#include <vector>

class AGameObject
{
protected:
    bool bEnabled;
    std::string strName;

    AGameObject* pParent;
    std::vector<AGameObject*> vecChildren;
    std::vector<AComponent*> vecComponent;

public:
    AGameObject(std::string strName);
    virtual ~AGameObject() = default;

public:
    virtual void initialize() = 0;
    virtual void processInput(SDL_Event eEvent);
    virtual void update();
    virtual void draw(SDL_Window* pWindow);

public:
    void attachChild(AGameObject* pChild);
    void detachChild(AGameObject* pChild);
    AGameObject* findChildByName(std::string strName);

    void attachComponent(AComponent* pAComponent);
    void detachComponent(AComponent* pAComponent);
    AComponent* findComponentByName(std::string strName);
    std::vector<AComponent*> getComponents(ComponentType EType);
    std::vector<AComponent*> getComponentsRecursively(ComponentType EType, bool bInclusive = true);

public:
    bool isEnabled();
    void setEnabled(bool bEnabled);
    std::string getName();
    AGameObject* getParent();
    void setParent(AGameObject* pParent);
};
