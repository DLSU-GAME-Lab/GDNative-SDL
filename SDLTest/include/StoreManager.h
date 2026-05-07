#pragma once
#include "unordered_map"
#include "string"
#include "EventListener.h"
class StoreManager :public EventListener
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
	EventKey EKey;  
    std::unordered_map<std::string,StoreItem> storeItems;
    bool bListenerEnabled;


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
    StoreManager();
    StoreManager(const StoreManager&) {};
    StoreManager& operator = (const StoreManager&) {};

public:
    static void initialize();
    static void destroy();

    static StoreManager* getInstance();

    // Inherited via EventListener
    void onEventTrigger(std::unordered_map<std::string, void*> mapParameter) override;
    EventKey getKey() override;
    bool isListenerEnabled() override;
    void setListenerEnabled(bool bListenerEnabled) override;
    std::string getListenerOwnerName() override;
    /* * * * * * * * * * * * * * * * * * * * */
};

