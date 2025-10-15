#include "TrackerManager.h"
#include "GameObjectManager.h"
TrackerManager* TrackerManager::P_SHARED_INSTANCE = NULL;

TrackerManager::TrackerManager(std::string strName, AGameObject* pOwner) :AComponent(strName, ComponentType::SCRIPT)
{
	this->EKey = EventKey::COLOR_MATCH;
	this->attachOwner(pOwner);
}
TrackerManager* TrackerManager::getInstance()
{
	return P_SHARED_INSTANCE;
}
void TrackerManager::initialize(std::string strName, AGameObject* pOwner)
{
	P_SHARED_INSTANCE = new TrackerManager(strName, pOwner);
}
void TrackerManager::destroy()
{
	delete P_SHARED_INSTANCE;
}
void TrackerManager::perform()
{

}
void TrackerManager::registerTracker(Tracker* pTracker)
{
	this->vecTracker.push_back(pTracker);
}

void TrackerManager::unRegisterTracker(Tracker* pTracker)
{
	int nIndex = this->findTracker(pTracker);
	if(nIndex != -1) {
		this->vecTracker.erase(this->vecTracker.begin() + nIndex);
	}
}

void TrackerManager::unRegisterAllTracker()
{
	for (Tracker* pTracker : this->vecTracker)
		this->unRegisterTracker(pTracker);

	this->vecTracker.clear();
}

int TrackerManager::findTracker(Tracker* pTracker)
{
	int nIndex = -1;
	for (int i = 0; i < this->vecTracker.size() && nIndex == -1; i++) {
		if (this->vecTracker[i] == pTracker)
			nIndex = i;
	}

	return nIndex;
}

void TrackerManager::onEventTrigger(std::unordered_map<std::string, void*> mapParameter)
{
	GemType EType;
	int nNumber;
	if (mapParameter.find("GemType") != mapParameter.end() && mapParameter.find("RemovedNumber") != mapParameter.end())
	{
		EType = *static_cast<GemType*>(mapParameter["GemType"]);
		nNumber = *static_cast<int*>(mapParameter["RemovedNumber"]);
	}

	for (Tracker* pTracker : this->vecTracker)
	{
		if (pTracker->getGemType() == EType)
		{
			pTracker->updateScore(nNumber);
		}
	}

}

EventKey TrackerManager::getKey()
{
	return this->EKey;
}
