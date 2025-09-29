#pragma once

class Collider;
class ACollisionListener {
public:
    ~ACollisionListener() {}

public:
    virtual void onCollisionEnter(Collider* pCollider) = 0;
    virtual void onCollisionContinue(Collider* pCollider) = 0;
    virtual void onCollisionExit(Collider* pCollider) = 0;
};