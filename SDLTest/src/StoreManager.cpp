#include "StoreManager.h"
bool StoreManager::purchaseItem(std::string itemID)
{
    if (itemID == "Coins")
    {
		// For testing purposes only, purchasing coins will give player 100 coins for free
        this->addCoins(100);
        return true;
    }
    
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

void StoreManager::initialize()
{
    P_SHARED_INSTANCE = new StoreManager();
    //Initialize font loading
   
}

void StoreManager::destroy()
{
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
/* * * * * * * * * * * * * * * * * * * * */