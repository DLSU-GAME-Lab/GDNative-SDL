#pragma once
#include "vector"
#include "math.h"
#include "AComponent.h"
#include "AGameObject.h"
#include "EnumComponentType.h"
#include "ACollisionListener.h"
//enum  class ColliderShape
//{
//    RECTANGLE = 0,
//    CIRCLE
//};
//struct ColliderBounds {
//    ColliderShape type;
//    SDL_FRect rect;     // Used if type == Rectangle
//    SDL_FPoint center;  // Used if type == Circle
//    float radius;       // Used if type == Circle
//};

class ACollider: public AComponent
{
protected:
    ACollisionListener* pListener;
    std::vector<ACollider*> vecCollided;
    bool bCleanUp;
    bool bFollowParent;
    bool bCollideLeft;
    bool bCollideRight;
    bool bCollideTop;
    bool bCollideBottom;
public:
    ACollider(std::string strName, bool bFollowParent = true);
public:
    virtual void perform() = 0;
    virtual bool isColliding(ACollider* pCollider) = 0;

    int findCollider(ACollider* pCollider);
    void cleanCollisions();

    void onCollisionEnter(ACollider* pCollider);
    void onCollisionContinue(ACollider* pCollider);
    void onCollisionExit(ACollider* pCollider);

public:
    void setListener(ACollisionListener* pListener);
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


