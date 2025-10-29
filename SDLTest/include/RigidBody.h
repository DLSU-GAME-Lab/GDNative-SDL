#pragma once
#include "BoxCollider.h"
class RigidBody : public BoxCollider
{
private:
    const float F_GRAVITY = 9.81f;

    float fWeight;
    float fDrag;
    bool bGravityEnabled;
    bool bGrounded;
    Vector2D velocity;

public:
	RigidBody();

    void addForce(Vector2D force);

    virtual void onCollisionEnter(ACollider* pCollider) override;
    virtual void onCollisionContinue(ACollider* pCollider) override;
    virtual void onCollisionExit(ACollider* pCollider) override;
    void onUpdate();

public:
    void setWeight(float fWeight);
    void setDrag(float fDrag);
    void setVelocity(Vector2D velocity);
    void setGravityEnabled(bool bGravityEnabled);

    float getWeight() const;
    float getDrag() const;
    Vector2D getVelocity() const;
    bool getGravityEnabled() const;
};

