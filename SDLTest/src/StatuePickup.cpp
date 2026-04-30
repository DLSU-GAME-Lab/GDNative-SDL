#include "StatuePickup.h"
#include "unordered_map"
#include "EnumEventKey.h"
#include "EventBroadcaster.h"
#include "GameObjectManager.h"
StatuePickup::StatuePickup():AInteractable("StatuePickup")
{
}

void StatuePickup::perform()
{
}

void StatuePickup::onInteract()
{
	std::unordered_map <std::string, void*> mapParam;
	EventBroadcaster::getInstance()->broadcast(EventKey::END_LEVEL);
	this->pOwner->setEnabled(false);
	this->bCleanUp = true;
	//PhysicsSystem::getInstance()->cleanUp();
	GameObjectManager::getInstance()->deleteObject(this->pOwner);
}
