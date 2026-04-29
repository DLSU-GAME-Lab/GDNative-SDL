#pragma once
#include "unordered_map"
#include "string"
class StoreManager
{
public:
    struct StoreItem
    {
        int nPrice;
        std::string strName;
		std::string strTextureKey;
    };
private:
    int nCoins;
    std::unordered_map<std::string,StoreItem> storeItems;

public:
    bool purchaseItem(std::string itemID);
	void addStoreItem(std::string itemID, int nPrice, std::string strTextureKey);
	void unloadStoreItems();
    int getCoins() const;
    void addCoins(int amount);
	void subtractCoins(int amount);
	//for testing purposes only, prints all store items to console
    void printStoreItems();

/* * * * * * * * * * * * * * * * * * * * *
*       SINGLETON-RELATED CONTENT       *
* * * * * * * * * * * * * * * * * * * * */
private:
    static StoreManager* P_SHARED_INSTANCE;

private:
    StoreManager() {};
    StoreManager(const StoreManager&) {};
    StoreManager& operator = (const StoreManager&) {};

public:
    static void initialize();
    static void destroy();

    static StoreManager* getInstance();
    /* * * * * * * * * * * * * * * * * * * * */
};

