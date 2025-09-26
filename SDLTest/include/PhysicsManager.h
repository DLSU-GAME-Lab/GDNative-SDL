#pragma once
#include "AGameObject.h"
#include "AComponent.h"
#include "EnumComponentType.h"
#include "Collider.h"
class PhysicsManager: AComponent
{
private:
    std::vector<Collider*> vecTrackedCollider;
    std::vector<Collider*> vecUntrackedCollider;

public:
    void onAttach() override;
    void perform() override;
    void trackCollider(Collider* pCollider);
    void untrackCollider(Collider* pCollider);
    void cleanUp();

private:
    int findTrackedCollider(Collider* pCollider);

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
    static void initialize(std::string strName, AGameObject* pParent);
    static void destroy();
    /* * * * * * * * * * * * * * * * * * * * */
};

