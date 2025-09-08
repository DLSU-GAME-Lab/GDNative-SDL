#pragma once

#include "AComponent.h"
#include "SpriteRenderer.h"
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

    // X and Y since 2d
    float fPosX, fPosY;
    float fScaleX, fScaleY;
    float fRot;

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

    void attachComponent(AComponent* pComponent);
    void detachComponent(AComponent* pComponent);
    AComponent* findComponentByName(std::string strName);
    std::vector<AComponent*> getComponents(ComponentType EType);
    std::vector<AComponent*> getComponentsRecursively(ComponentType EType, bool bInclusive = true);

public:
    bool isEnabled() const;
    void setEnabled(bool bEnabled);
    std::string getName() const;
    AGameObject* getParent() const;
    void setParent(AGameObject* pParent);
    void setPosX(float fX);
    float getPosX();
    void setPosY(float fY);
    float getPosY();
    void setScaleX(float fX);
    float getScaleX();
    void setScaleY(float fY);
    float getScaleY();
    void setRot(float fRot);
    float getRot();


};
