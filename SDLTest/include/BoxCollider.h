#pragma once
#include "AComponent.h"
#include "ACollider.h"
#include "AGameObject.h"
#include "EnumComponentType.h"
#include "ICollisionListener.h"
#include "vector"
#include "math.h"
struct RectangleShape {
    SDL_FRect rect;
    SDL_Color fillColor;

    RectangleShape()
    {
        rect = { 0, 0, 0, 0 };
        fillColor = { 255, 255, 255, 255 }; // default white
    }
    RectangleShape(float width, float height) {
        rect = { 0, 0, width, height };
        fillColor = { 255, 255, 255, 255 }; // default white
    }
    SDL_FRect getGlobalBounds()
    {
        return rect;
    }
    void setPosition(float centerX, float centerY) {
        rect.x = centerX - rect.w / 2.0f;
        rect.y = centerY - rect.h / 2.0f;


    }

    void setSize(float width, float height) {
        float centerX = rect.x + rect.w / 2.0f;
        float centerY = rect.y + rect.h / 2.0f;
        rect.w = width;
        rect.h = height;
        setPosition(centerX, centerY);


    }

    void setFillColor(SDL_Color color) {
        fillColor = color;
    }

    void draw(SDL_Renderer* renderer) const {
        SDL_SetRenderDrawColor(renderer, fillColor.r, fillColor.g, fillColor.b, fillColor.a);
        SDL_RenderFillRect(renderer, &rect);
    }
};

class BoxCollider : public ACollider
{
protected:
    SDL_FRect size;
    RectangleShape rectShape;
public:
    BoxCollider(std::string strName);

public:
    virtual void onAttach() override;
    void perform() override;
    bool isColliding(ACollider* pCollider) override;

public:
    SDL_FRect getSize() const;
    void setSize(SDL_FRect size);
   
    SDL_FRect getGlobalBounds();
};

