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
public:
    BoxCollider(std::string strName);

public:
    virtual void onAttach() override;
    void perform() override;
    bool isColliding(ACollider* pCollider) override;

public:
    Vector2D getSize() const;
    void setSize(Vector2D size);
   
    Vector2D getOffset() const;
    void setOffset(Vector2D size);
   
    SDL_FRect getGlobalBounds();
};

