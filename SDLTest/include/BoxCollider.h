#pragma once
#include "AComponent.h"
#include "ACollider.h"
#include "AGameObject.h"
#include "EnumComponentType.h"
#include "ICollisionListener.h"
#include "vector"
#include "math.h"

class BoxCollider : public ACollider
{
protected:
    Vector2D size;
    Vector2D offset;

    bool bCollideLeft;
    bool bCollideRight;
    bool bCollideBottom;
    bool bCollideTop;

public:
    BoxCollider(std::string strName);

public:
    virtual void onAttach() override;
    void perform() override;
    bool isColliding(ACollider* pCollider) override;

    virtual void onCollisionEnter(ACollider* pCollider) override;
    virtual void onCollisionContinue(ACollider* pCollider) override;
    virtual void onCollisionExit(ACollider* pCollider) override;

public:
    Vector2D getSize() const;
    void setSize(Vector2D size);
   
    Vector2D getOffset() const;
    void setOffset(Vector2D size);

    bool isCollidedLeft() const;
    bool isCollidedRight() const;
    bool isCollidedBottom() const;
    bool isCollidedTop() const;
   
    SDL_FRect getGlobalBounds();
};

