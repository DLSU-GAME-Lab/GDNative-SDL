#pragma once
#include "BoxCollider.h"
class RigidBody : public BoxCollider
{
private:
    const float F_GRAVITY = 9.81f;

    float fWeight;
    bool bGrounded;
    Vector2D velocity;

public:
	RigidBody();

    void move(Vector2D force);

    virtual void onCollisionEnter(ACollider* pCollider) override;
    virtual void onCollisionContinue(ACollider* pCollider) override;
    virtual void onCollisionExit(ACollider* pCollider) override;
    void onUpdate();

public:
    void setWeight(float fWeight);
    void setVelocity(Vector2D velocity);

    float getWeight() const;
    Vector2D getVelocity() const;
};

