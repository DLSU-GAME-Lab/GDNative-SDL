#include "DataAssetManager.h"

void DataAssetManager::addDataAsset(std::string strName, ADataAsset* pDataAsset)
{
    if (this->mapDataAsset.find(strName) != this->mapDataAsset.end())
    {
        delete this->mapDataAsset[strName];
    }

    this->mapDataAsset[strName] = pDataAsset;
    pDataAsset->initialize();
}

void DataAssetManager::removeDataAsset(std::string strName)
{
    if (this->mapDataAsset.find(strName) != this->mapDataAsset.end())
    {
        ADataAsset* pDataAsset = this->mapDataAsset[strName];
        this->mapDataAsset.erase(strName);

        delete pDataAsset;
    }
}

ADataAsset* DataAssetManager::getDataAsset(std::string strName)
{
    if (this->mapDataAsset.find(strName) != this->mapDataAsset.end())
    {
        return this->mapDataAsset[strName];
    }

    return nullptr;
}

/* * * * * * * * * * * * * * * * * * * * *
 *       SINGLETON-RELATED CONTENT       *
 * * * * * * * * * * * * * * * * * * * * */
DataAssetManager* DataAssetManager::P_SHARED_INSTANCE = nullptr;

void DataAssetManager::initialize()
{
    P_SHARED_INSTANCE = new DataAssetManager();
}

void DataAssetManager::destroy()
{
    for (auto& pair : P_SHARED_INSTANCE->mapDataAsset)
    {
        delete pair.second;
	}

    delete P_SHARED_INSTANCE;
}

DataAssetManager* DataAssetManager::getInstance()
{
    return P_SHARED_INSTANCE;
}
/* * * * * * * * * * * * * * * * * * * * */
