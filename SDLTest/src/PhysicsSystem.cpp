#include "PhysicsSystem.h"
#include "EmptyObject.h"
#include "GameObjectManager.h"
#include "RigidBody.h"

void PhysicsSystem::perform() 
{
    this->checkCollision();
}

void PhysicsSystem::checkCollision()
{
    ACollider* pColliderA = NULL;
    ACollider* pColliderB = NULL;

    for (int i = 0; i < this->vecTrackedCollider.size(); i++) {
        pColliderA = this->vecTrackedCollider[i];

        for (int j = 0; j < this->vecTrackedCollider.size(); j++) {
            pColliderB = this->vecTrackedCollider[j];
            if (pColliderA != pColliderB) {

                if (pColliderA->isColliding(pColliderB) &&
                    !pColliderA->hasCollided(pColliderB) &&
                    !pColliderB->hasCollided(pColliderA)) {

                    pColliderA->setCollided(pColliderB, true);
                    pColliderB->setCollided(pColliderA, true);
                    pColliderA->onCollisionEnter(pColliderB);
                    pColliderB->onCollisionEnter(pColliderA);
                }

                else if (!pColliderA->isColliding(pColliderB) &&
                    pColliderA->hasCollided(pColliderB) &&
                    pColliderB->hasCollided(pColliderA)) {

                    pColliderA->setCollided(pColliderB, false);
                    pColliderB->setCollided(pColliderA, false);
                    pColliderA->onCollisionExit(pColliderB);
                    pColliderB->onCollisionExit(pColliderA);
                }

                if (pColliderA->isColliding(pColliderB) &&
                    pColliderA->hasCollided(pColliderB) &&
                    pColliderB->hasCollided(pColliderA)) {
                    pColliderA->onCollisionContinue(pColliderB);
                    pColliderB->onCollisionContinue(pColliderA);
                }
            }
        }

        if (RigidBody* pRigidBody = dynamic_cast<RigidBody*>(pColliderA))
        {
            pRigidBody->onUpdate();
        }
    }

    this->cleanUp();
}

void PhysicsSystem::trackCollider(ACollider* pCollider) {
    pCollider->cleanCollisions();
    this->vecTrackedCollider.push_back(pCollider);
}

void PhysicsSystem::untrackCollider(ACollider* pCollider) {
    this->vecUntrackedCollider.push_back(pCollider);
}

void PhysicsSystem::cleanUp() {
    ACollider* pCollider = NULL;
    int nIndex;

    for (ACollider* pCollider : this->vecTrackedCollider) {
        if (pCollider->isCleanUp())
        {
            this->vecUntrackedCollider.push_back(pCollider);
            std::cout << "Collider: " << pCollider->getName() << " is being removed from tracking." << std::endl;
        }
    
    }

    for (int i = 0; i < this->vecUntrackedCollider.size(); i++) {
        pCollider = this->vecUntrackedCollider[i];
        nIndex = findTrackedCollider(pCollider);

        if (nIndex != -1)
            this->vecTrackedCollider.erase(this->vecTrackedCollider.begin() + nIndex);
    }

    this->vecUntrackedCollider.clear();
}

int PhysicsSystem::findTrackedCollider(ACollider* pCollider) {
    int nIndex = -1;
    for (int i = 0; i < this->vecTrackedCollider.size() && nIndex == -1; i++) {
        if (pCollider == this->vecTrackedCollider[i])
            nIndex = i;
    }

    return nIndex;
}

/* * * * * * * * * * * * * * * * * * * * *
 *       SINGLETON-RELATED CONTENT       *
 * * * * * * * * * * * * * * * * * * * * */
PhysicsSystem* PhysicsSystem::P_SHARED_INSTANCE = NULL;

PhysicsSystem::PhysicsSystem(std::string strName) : AComponent(strName, ComponentType::SCRIPT) {}
PhysicsSystem::PhysicsSystem(const PhysicsSystem& CObject) : AComponent(CObject.strName, ComponentType::SCRIPT) {}

PhysicsSystem* PhysicsSystem::getInstance() {
    return P_SHARED_INSTANCE;
}

void PhysicsSystem::initialize() {
    EmptyObject* pPhysManagerHolder = new EmptyObject("Physics Manager Holder");
    P_SHARED_INSTANCE = new PhysicsSystem("PhysicsSystem");
    pPhysManagerHolder->attachComponent(P_SHARED_INSTANCE);
    GameObjectManager::getInstance()->addObject(pPhysManagerHolder);
}

void PhysicsSystem::destroy() {
    delete P_SHARED_INSTANCE;
}
/* * * * * * * * * * * * * * * * * * * * */
