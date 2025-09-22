#pragma once

#include "EnumComponentType.h"
#include <SDL3/SDL.h>
#include <string>

class AGameObject;

class AComponent
{
protected:
    AGameObject* pOwner;
    ComponentType EType;
    std::string strName;
    float fDeltaTime;

public:
    AComponent(std::string strName, ComponentType EType);
    virtual ~AComponent();

public:
    void attachOwner(AGameObject* pOwner);
    void detachOwner();

public:
    virtual void perform() = 0;

public:
    void setDeltaTime(float fDeltaTime);
    AGameObject* getOwner();
    ComponentType getType() const;
    std::string getName() const;
};
