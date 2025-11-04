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
    bool bCollideLeft;
    bool bCollideRight;
    bool bCollideTop;
    bool bCollideBottom;
public:
    ACollider(std::string strName);
public:
    virtual void perform() = 0;
    virtual bool isColliding(ACollider* pCollider) = 0;

    int findCollider(ACollider* pCollider);
    void cleanCollisions();

    virtual void onCollisionEnter(ACollider* pCollider);
    virtual void onCollisionContinue(ACollider* pCollider);
    virtual void onCollisionExit(ACollider* pCollider);

public:
    void setListener(ICollisionListener* pListener);
    SDL_FRect getOffset();
    void setOffset(SDL_FRect COffset);
    bool hasCollided(ACollider* pCollider);
    void setCollided(ACollider* pCollider, bool bCollided);
    bool isCollidedLeft();
    bool isCollidedRight();
    bool isCollidedTop();
    bool isCollidedBottom();

    bool isCleanUp();
    void setCleanUp(bool bCleanUp);
    virtual SDL_FRect getGlobalBounds() = 0;
};


