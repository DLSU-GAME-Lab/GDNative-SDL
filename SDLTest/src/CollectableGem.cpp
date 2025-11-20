#include "CollectableGem.h"
#include "GameObjectManager.h"
#include "EventBroadcaster.h"
#include "AudioManager.h"
#include "PhysicsSystem.h"
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
	AudioManager::getInstance()->play(new AudioPlayer("Pickup", AudioGroupTag::SFX));
	PhysicsSystem::getInstance()->untrackCollider(this);
	this->pOwner->setEnabled(false);
	//GameObjectManager::getInstance()->deleteObject(this->pOwner);
}
