#pragma once
#include "ADataAsset.h"
#include <unordered_map>
#include <string>

class DataAssetManager
{
private:
	std::unordered_map<std::string, ADataAsset*> mapDataAsset;

public:
	void addDataAsset(std::string strName, ADataAsset* pDataAsset);
	void removeDataAsset(std::string strName);
	ADataAsset* getDataAsset(std::string strName);

    /* * * * * * * * * * * * * * * * * * * * *
     *       SINGLETON-RELATED CONTENT       *
     * * * * * * * * * * * * * * * * * * * * */
private:
    static DataAssetManager* P_SHARED_INSTANCE;

private:
    DataAssetManager() {};
    DataAssetManager(const DataAssetManager&) {};
    DataAssetManager& operator = (const DataAssetManager&) {};

public:
    static void initialize();
    static void destroy();

    static DataAssetManager* getInstance();
    /* * * * * * * * * * * * * * * * * * * * */
};

