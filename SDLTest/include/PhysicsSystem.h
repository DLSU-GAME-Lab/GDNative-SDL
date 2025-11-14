#pragma once
#include "AGameObject.h"
#include "AComponent.h"
#include "EnumComponentType.h"
#include "ACollider.h"
#include "RigidBody.h"

class PhysicsSystem: public AComponent
{
private:
    std::vector<ACollider*> vecTrackedCollider;
    std::vector<ACollider*> vecUntrackedCollider;
    std::vector<RigidBody*> vecRigidBody;
    float fDelayTicks = 1.0f;

public:
    void perform();
    void checkCollision();
    void trackCollider(ACollider* pCollider);
    void untrackCollider(ACollider* pCollider);
    void addRigidBody(RigidBody* pRigidBody);
    void cleanUp();

private:
    int findTrackedCollider(ACollider* pCollider);

    /* * * * * * * * * * * * * * * * * * * * *
     *       SINGLETON-RELATED CONTENT       *
     * * * * * * * * * * * * * * * * * * * * */
private:
    static PhysicsSystem* P_SHARED_INSTANCE;

private:
    PhysicsSystem(std::string strName);
    PhysicsSystem(const PhysicsSystem&);
    PhysicsSystem& operator = (const PhysicsSystem&);

public:
    static PhysicsSystem* getInstance();
    static void initialize();
    static void destroy();
    /* * * * * * * * * * * * * * * * * * * * */
};

