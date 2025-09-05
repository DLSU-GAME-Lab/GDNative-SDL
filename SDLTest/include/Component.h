#pragma once

#include "EnumComponentType.h"
#include <SDL3/SDL.h>
#include <string>

class GameObject;

class Component
{
protected:
    GameObject* pOwner;
    ComponentType EType;
    std::string strName;

public:
    Component(std::string strName, ComponentType EType);
    virtual ~Component();

public:
    void attachOwner(GameObject* pOwner);
    void detachOwner();

public:
    virtual void perform() = 0;

public:
    GameObject* getOwner();
    ComponentType getType() const;
    std::string getName() const;
};
