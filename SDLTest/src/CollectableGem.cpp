#include "CollectableGem.h"
#include "GameObjectManager.h"

CollectableGem::CollectableGem() : AInteractable("CollectableGem")
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
