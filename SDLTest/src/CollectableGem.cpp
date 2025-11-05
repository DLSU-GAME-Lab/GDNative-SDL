#include "CollectableGem.h"
#include "GameObjectManager.h"

CollectableGem::CollectableGem() : ACollectable("CollectableGem")
{

}

void CollectableGem::perform()
{

}

void CollectableGem::onCollect()
{
	std::cout << "Collected." << std::endl;
	//GameObjectManager::getInstance()->deleteObject(this->pOwner);
}
