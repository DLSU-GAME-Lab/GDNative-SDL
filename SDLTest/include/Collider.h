#pragma once
#include "AComponent.h"
#include "AGameObject.h"
#include "EnumComponentType.h"
#include "ACollisionListener.h"
#include "vector"
class Collider : public AComponent
{
private:
    ACollisionListener* pListener;
    SDL_FRect COffset;
    std::vector<Collider*> vecCollided;
    bool bCleanUp;
    bool bFollowParent;

public:
    Collider(std::string strName, bool bFollowParent = true);

public:
    void perform();
    bool isColliding(Collider* pCollider);

    int findCollider(Collider* pCollider);
    void cleanCollisions();

    void onCollisionEnter(Collider* pCollider);
    void onCollisionContinue(Collider* pCollider);
    void onCollisionExit(Collider* pCollider);

public:
    void setListener(ACollisionListener* pListener);
    SDL_FRect getOffset();
    void setOffset(SDL_FRect COffset);
    bool hasCollided(Collider* pCollider);
    void setCollided(Collider* pCollider, bool bCollided);

    bool isCleanUp();
    void setCleanUp(bool bCleanUp);
    SDL_FRect getGlobalBounds();
};

