#pragma once

class ACollider;
class ACollisionListener {
public:
    ~ACollisionListener() {}

public:
    virtual void onCollisionEnter(ACollider* pCollider) = 0;
    virtual void onCollisionContinue(ACollider* pCollider) = 0;
    virtual void onCollisionExit(ACollider* pCollider) = 0;
};