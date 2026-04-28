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