#pragma once
#include "AComponent.h"
#include "AGameObject.h"
#include "EnumComponentType.h"
#include "ICollisionListener.h"
#include <vector>
#include <cmath>

class ACollider: public AComponent
{
protected:
    ICollisionListener* pListener;
    std::vector<ACollider*> vecCollided;
    Vector2D intersection;
    bool bCleanUp;
    bool bIsTrigger;

public:
    ACollider(std::string strName);
public:
    virtual void perform() = 0;
    virtual bool isColliding(ACollider* pCollider) = 0;

    int findCollider(ACollider* pCollider);
    virtual void cleanCollisions();

    virtual void onCollisionEnter(ACollider* pCollider);
    virtual void onCollisionContinue(ACollider* pCollider);
    virtual void onCollisionExit(ACollider* pCollider);

public:
    void setListener(ICollisionListener* pListener);
    bool getIsTrigger() const;
    void setIsTrigger(bool bIsTrigger);
    bool hasCollided(ACollider* pCollider);
    void setCollided(ACollider* pCollider, bool bCollided);

    bool isCleanUp();
    void setCleanUp(bool bCleanUp);
    virtual SDL_FRect getGlobalBounds() = 0;
};


