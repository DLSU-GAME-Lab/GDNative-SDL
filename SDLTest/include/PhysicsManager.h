#pragma once
#include "AGameObject.h"
#include "AComponent.h"
#include "EnumComponentType.h"
#include "ACollider.h"
#include "Gravity.h"
class PhysicsManager: AComponent
{
private:
    std::vector<ACollider*> vecTrackedCollider;
    std::vector<ACollider*> vecUntrackedCollider;
public:
    void perform();
    void checkCollision();
    void trackCollider(ACollider* pCollider);
    void untrackCollider(ACollider* pCollider);
    void cleanUp();

private:
    int findTrackedCollider(ACollider* pCollider);

    /* * * * * * * * * * * * * * * * * * * * *
     *       SINGLETON-RELATED CONTENT       *
     * * * * * * * * * * * * * * * * * * * * */
private:
    static PhysicsManager* P_SHARED_INSTANCE;

private:
    PhysicsManager(std::string strName);
    PhysicsManager(const PhysicsManager&);
    PhysicsManager& operator = (const PhysicsManager&);

public:
    static PhysicsManager* getInstance();
    static void initialize();
    static void destroy();
    /* * * * * * * * * * * * * * * * * * * * */
};

