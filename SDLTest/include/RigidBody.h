#pragma once
#include "BoxCollider.h"
class RigidBody : public BoxCollider
{
private:
    const float F_GRAVITY = 7.5f;

    float fWeight;
    float fDrag;
    bool bGravityEnabled;
    bool bGrounded;
    Vector2D force;
    Vector2D velocity;
    float fTicks = 0.0f;

public:
	RigidBody();

    void addForce(Vector2D force, bool instant = false);

    virtual void onCollisionEnter(ACollider* pCollider) override;
    virtual void onCollisionContinue(ACollider* pCollider) override;
    virtual void onCollisionExit(ACollider* pCollider) override;
    virtual void cleanCollisions() override;
    void physicsUpdate();
    void physicsLateUpdate();

public:
    void setWeight(float fWeight);
    void setDrag(float fDrag);
    void setVelocity(Vector2D velocity);
    void setGravityEnabled(bool bGravityEnabled);

    float getWeight() const;
    float getDrag() const;
    Vector2D getVelocity() const;
    bool getGravityEnabled() const;
    bool getGrounded() const;
};

