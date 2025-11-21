#include "TrackerManager.h"
#include "GameObjectManager.h"
TrackerManager* TrackerManager::P_SHARED_INSTANCE = NULL;

TrackerManager::TrackerManager(std::string strName, AGameObject* pOwner) :AComponent(strName, ComponentType::SCRIPT)
{
	this->EKey = EventKey::COLOR_MATCH;
	this->attachOwner(pOwner);
	this->bVictory = false;
	this->bTrackerEnabled = true;
	this->bListenerEnabled = true;

}
TrackerManager* TrackerManager::getInstance()
{
	return P_SHARED_INSTANCE;
}
void TrackerManager::initialize(std::string strName, AGameObject* pOwner)
{
	P_SHARED_INSTANCE = new TrackerManager(strName, pOwner);
	EventBroadcaster::getInstance()->registerListener(P_SHARED_INSTANCE);
}
void TrackerManager::destroy()
{
	EventBroadcaster::getInstance()->unregisterListener(P_SHARED_INSTANCE);
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

void TrackerManager::disableTracker()
{
	this->bTrackerEnabled = false;
}

bool TrackerManager::CheckVictory()
{
	for (Tracker* pTracker : this->vecTracker)
	{
		if (!pTracker->isZero())
		{
			std::cout << pTracker->getName() + " is not Zero" << std::endl;
			return false;
		}
		std::cout << pTracker->getName() + " is Zero" << std::endl;

	}
	return true;
}

void TrackerManager::onEventTrigger(std::unordered_map<std::string, void*> mapParameter)
{
	GemType EType;
	int nNumber;
	if(this->bTrackerEnabled)
	{
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
		this->bVictory = this->CheckVictory();
		if (this->bVictory)
		{
			std::unordered_map<std::string, void*> victoryParams;

			//broadcast event for victory screen
			victoryParams["isVictory"] = static_cast<void*>(&this->bVictory);
			EventBroadcaster::getInstance()->broadcast(EventKey::GAME_END, victoryParams);
		}
	}


}

EventKey TrackerManager::getKey()
{
	return this->EKey;
}

bool TrackerManager::isListenerEnabled()
{
	return this->bListenerEnabled;
}

void TrackerManager::setListenerEnabled(bool bListenerEnabled)
{
	this->bListenerEnabled = bListenerEnabled;
}

std::string TrackerManager::getListenerOwnerName()
{
	return this->strName;
}
