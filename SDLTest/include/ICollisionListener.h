#pragma once

class ACollider;
class ICollisionListener {
public:
    ~ICollisionListener() {}

public:
    virtual void onCollisionEnter(ACollider* pCollider) = 0;
    virtual void onCollisionContinue(ACollider* pCollider) = 0;
    virtual void onCollisionExit(ACollider* pCollider) = 0;
};