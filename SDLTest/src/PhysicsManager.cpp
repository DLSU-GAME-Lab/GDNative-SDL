#include "PhysicsManager.h"

void PhysicsManager::perform() {

    Collider* pColliderA = NULL;
    Collider* pColliderB = NULL;

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
    }

    this->cleanUp();
}

void PhysicsManager::trackCollider(Collider* pCollider) {
    pCollider->cleanCollisions();
    this->vecTrackedCollider.push_back(pCollider);
}

void PhysicsManager::untrackCollider(Collider* pCollider) {
    this->vecUntrackedCollider.push_back(pCollider);
}

void PhysicsManager::cleanUp() {
    Collider* pCollider = NULL;
    int nIndex;

    for (Collider* pCollider : this->vecTrackedCollider) {
        if (pCollider->isCleanUp())
            this->vecUntrackedCollider.push_back(pCollider);
    }

    for (int i = 0; i < this->vecUntrackedCollider.size(); i++) {
        pCollider = this->vecUntrackedCollider[i];
        nIndex = findTrackedCollider(pCollider);

        if (nIndex != -1)
            this->vecTrackedCollider.erase(this->vecTrackedCollider.begin() + nIndex);
    }

    this->vecUntrackedCollider.clear();
}

int PhysicsManager::findTrackedCollider(Collider* pCollider) {
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
PhysicsManager* PhysicsManager::P_SHARED_INSTANCE = NULL;

PhysicsManager::PhysicsManager(std::string strName) : AComponent(strName, ComponentType::SCRIPT) {}
PhysicsManager::PhysicsManager(const PhysicsManager& CObject) : AComponent(CObject.strName, ComponentType::SCRIPT) {}

PhysicsManager* PhysicsManager::getInstance() {
    return P_SHARED_INSTANCE;
}

void PhysicsManager::initialize(std::string strName, AGameObject* pParent) {
    P_SHARED_INSTANCE = new PhysicsManager(strName);
    pParent->attachComponent(P_SHARED_INSTANCE);
}

void PhysicsManager::destroy() {
    delete P_SHARED_INSTANCE;
}
/* * * * * * * * * * * * * * * * * * * * */
