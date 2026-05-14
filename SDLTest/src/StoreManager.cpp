#include "StoreManager.h"
#include "iostream"
#include "EventBroadcaster.h"
bool StoreManager::purchaseItem(std::string itemID)
{
    if (itemID == "Coins")
    {
        this->addCoins(100);
        return true;
    }

    auto it = this->storeItems.find(itemID);
    if (it == this->storeItems.end())
    {
        std::cout << "[StoreManager] Item not found: " << itemID << std::endl;
        return false;
    }

    StoreItem& item = it->second;

    if (this->nCoins < item.nPrice)
    {
        std::cout << "[StoreManager] Not enough coins to purchase: " << item.strName << std::endl;
        return false;
    }

    this->nCoins -= item.nPrice;
    std::cout << "[StoreManager] Purchased: " << item.strName << std::endl;
    return true;
    
}

void StoreManager::addStoreItem(std::string itemID, int nPrice, std::string strTextureKey)
{
    StoreItem newItem;
    newItem.nPrice = nPrice;
	newItem.strName = itemID;
    newItem.strTextureKey = strTextureKey;
    this->storeItems[itemID] = newItem;
}

void StoreManager::unloadStoreItems()
{
	this->storeItems.clear();
}

int StoreManager::getCoins() const
{
    return this->nCoins;
}

void StoreManager::addCoins(int amount)
{
    this->nCoins += amount;
}

void StoreManager::subtractCoins(int amount)
{
	this->nCoins -= amount;
}
void StoreManager::printStoreItems()
{
    for (const auto& pair : this->storeItems)
    {
        printf("ID: %s | Name: %s | Price: %d | Texture: %s\n",
            pair.first.c_str(),
            pair.second.strName.c_str(),
            pair.second.nPrice,
            pair.second.strTextureKey.c_str());
    }
}
/* * * * * * * * * * * * * * * * * * * * *
 *       SINGLETON-RELATED CONTENT       *
 * * * * * * * * * * * * * * * * * * * * */
StoreManager* StoreManager::P_SHARED_INSTANCE = NULL;

StoreManager::StoreManager()
{
	this->EKey = EventKey::SHOP_PURCHASE;
	this->bListenerEnabled = true;
}

void StoreManager::initialize()
{
    P_SHARED_INSTANCE = new StoreManager();
    //Initialize font loading
	EventBroadcaster::getInstance()->registerListener(P_SHARED_INSTANCE);
   
}

void StoreManager::destroy()
{
    EventBroadcaster::getInstance()->unregisterListener(P_SHARED_INSTANCE);
	P_SHARED_INSTANCE->unloadStoreItems();
    if (P_SHARED_INSTANCE)
    {
        delete P_SHARED_INSTANCE;
        P_SHARED_INSTANCE = nullptr;
    }

}

StoreManager* StoreManager::getInstance()
{
    return P_SHARED_INSTANCE;
}
void StoreManager::onEventTrigger(std::unordered_map<std::string, void*> mapParameter)
{
    std::cout << "Coins: " << this->getCoins() << "\n";
    std::string itemID = *static_cast<std::string*>(mapParameter["Sender"]);

	this->purchaseItem(itemID);
	std::cout << "Coins: " << this->getCoins() << "\n";
}
EventKey StoreManager::getKey()
{
    return this->EKey;
}
bool StoreManager::isListenerEnabled()
{
    return this->bListenerEnabled;
}
void StoreManager::setListenerEnabled(bool bListenerEnabled)
{
    this->bListenerEnabled = bListenerEnabled;
}
std::string StoreManager::getListenerOwnerName()
{
    return "StoreManager";
}
/* * * * * * * * * * * * * * * * * * * * */