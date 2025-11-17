#include "CollectableGem.h"
#include "GameObjectManager.h"
#include "EventBroadcaster.h"
CollectableGem::CollectableGem() : AInteractable("CollectableGem")
{
	
}

void CollectableGem::perform()
{

}

void CollectableGem::onInteract()
{
	std::unordered_map <std::string, void*> mapParam;
	std::string strName = pOwner->getName();
	mapParam["GemName"] = static_cast<void*>(&strName);
	EventBroadcaster::getInstance()->broadcast(EventKey::ITEM_PICKUP, mapParam);
}
