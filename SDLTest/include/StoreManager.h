#pragma once
#include "vector"
#include "string"
class StoreManager
{
public:
    struct StoreItem
    {
        int nPrice;
        std::string strName;
    };
private:
    int nCoins;
    std::vector<StoreItem> storeItems;

public:
    bool purchaseItem(std::string itemID);
    int getCoins() const;
    void addCoins(int amount);
	void subtractCoins(int amount);

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

