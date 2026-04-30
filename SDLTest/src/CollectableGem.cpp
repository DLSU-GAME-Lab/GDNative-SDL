#include "CollectableGem.h"
#include "GameObjectManager.h"
#include "EventBroadcaster.h"
#include "AudioManager.h"
#include "PhysicsSystem.h"
#include "InventoryGUI.h"
#include "DataAssetManager.h"
#include "CollectableGemDataAsset.h"

CollectableGem::CollectableGem() : AInteractable("CollectableGem")
{
	
}

void CollectableGem::perform()
{

}

void CollectableGem::onInteract()
{
	CollectableGemDataAsset* pDataAsset = (CollectableGemDataAsset*)DataAssetManager::getInstance()->getDataAsset("CollectableGemDataAsset");
	pDataAsset->setGemPickedUp(this->pOwner->getName(), true);

	std::unordered_map <std::string, void*> mapParam;
	std::string strGemName = this->pOwner->getName();
	mapParam["GemName"] = static_cast<void*>(&strGemName);
	EventBroadcaster::getInstance()->broadcast(EventKey::ITEM_PICKUP, mapParam);
	
	AudioManager::getInstance()->play(new AudioPlayer("Pickup", AudioGroupTag::SFX));
	this->pOwner->setEnabled(false);
	this->bCleanUp = true;
	//PhysicsSystem::getInstance()->cleanUp();
	GameObjectManager::getInstance()->deleteObject(this->pOwner);
}
