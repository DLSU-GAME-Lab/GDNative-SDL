#pragma once
#include "AComponent.h"
#include "AGameObject.h"
#include "EnumComponentType.h"
#include "ACollisionListener.h"
#include "vector"
#include "math.h"
struct RectangleShape {
    SDL_FRect rect;
    SDL_Color fillColor;
    SDL_FPoint pivot = { 0.0f, 0.0f }; // relative to rect

    RectangleShape()
    { }
    RectangleShape(float width, float height) {
        rect = { 0, 0, width, height };
        fillColor = { 255, 255, 255, 255 }; // default white
        pivot = { width / 2.0f, height / 2.0f }; // center pivot
    }
    SDL_FRect getGlobalBounds()
    {
        return rect;
    }
    void setPosition(float x, float y) {
        rect.x = x - pivot.x;
        rect.y = y - pivot.y;

    }

    void setSize(float width, float height) {
        rect.w = width;
        rect.h = height;
        pivot = { width / 2.0f, height / 2.0f }; // update pivot

    }

    void setFillColor(SDL_Color color) {
        fillColor = color;
    }

    void draw(SDL_Renderer* renderer) const {
        SDL_SetRenderDrawColor(renderer, fillColor.r, fillColor.g, fillColor.b, fillColor.a);
        SDL_RenderFillRect(renderer, &rect);
    }
};

class Collider : public AComponent
{
private:
    ACollisionListener* pListener;
    SDL_FRect COffset;
    std::vector<Collider*> vecCollided;
    bool bCleanUp;
    bool bFollowParent;
    bool bCollideLeft;
    bool bCollideRight;
    bool bCollideTop;
    bool bCollideBottom;

public:
    Collider(std::string strName, bool bFollowParent = true);

public:
    void onAttach() override;
    void perform() override;
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
    bool isCollidedLeft();
    bool isCollidedRight();
    bool isCollidedTop();
    bool isCollidedBottom();

    bool isCleanUp();
    void setCleanUp(bool bCleanUp);
    SDL_FRect getGlobalBounds();
};

