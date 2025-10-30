#pragma once

#include "AComponent.h"
#include "Vector2D.h"
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
    bool bIsScreenObject;
    bool bFollowParent;
    Vector2D fVecTranslate;
    Vector2D fVecScale;
    float fRot;
    Vector2D fWindowSize;
public:
    AGameObject(std::string strName);
    virtual ~AGameObject();

public:
    virtual void initialize() = 0;
    virtual void processInput(SDL_Event* eEvent);
    virtual void update(float fDeltaTime);
    virtual void draw(SDL_Renderer* pRenderer);
    virtual SDL_FRect getGlobalBounds();

public:
    void attachChild(AGameObject* pChild);
    void detachChild(AGameObject* pChild);
    AGameObject* findChildByName(std::string strName);

    void attachComponent(AComponent* pComponent);
    void detachComponent(AComponent* pComponent);
    AComponent* findComponentByName(std::string strName);
    std::vector<AComponent*> getComponents(ComponentType EType);
    std::vector<AComponent*> getComponentsRecursively(ComponentType EType, bool bInclusive = true);
    bool componentExists(std::string strName);

public:
    bool getEnabled() const;
    void setEnabled(bool bEnabled);
    std::string getName() const;
    AGameObject* getParent() const;
    void setParent(AGameObject* pParent);
    void setPos(Vector2D fVecTranslate);
    void setScale(Vector2D fVecScale);
    Vector2D getPos();
    Vector2D getScale();
    void setRot(float fRot);
    float getRot();

    bool getIsScreenObject() const;
    void setIsScreenObject(bool bIsScreenObject);
    bool getFollowParent();
    void setFollowParent(bool bFollowParent);
    bool isGloballyEnabled() const;
 

};
