#include "EventBroadcaster.h"


void EventBroadcaster::registerListener(EventListener* pListener) {
    EventKey EKey = pListener->getKey();
    this->mapListener[EKey].push_back(pListener);
    this->vecListener.push_back(pListener);
}

void EventBroadcaster::unregisterListener(EventListener* pListener) {
    EventKey EKey = pListener->getKey();
    std::vector<EventListener*> vecListener = this->mapListener[EKey];
    int nIndex = this->findListener(pListener, vecListener);

    this->mapListener[EKey].erase(this->mapListener[EKey].begin() + nIndex);

    nIndex = this->findListener(pListener);
    if(nIndex != -1) {
        this->vecListener.erase(this->vecListener.begin() + nIndex);
    }
}

void EventBroadcaster::unregisterAllListeners() {
    for(EventListener* pListener : this->vecListener)
        this->unregisterListener(pListener);

    this->vecListener.clear();
    this->mapListener.clear();
}

void EventBroadcaster::broadcast(EventKey EKey, std::unordered_map<std::string, void*> mapParameter) {
    for(int i = 0; i < this->mapListener[EKey].size(); i++) {
        if(this->mapListener[EKey][i]->isListenerEnabled())
            this->mapListener[EKey][i]->onEventTrigger(mapParameter);
    }
}

void EventBroadcaster::disableOtherListenerExcept(EventListener* pListenerExcluded)
{
    for (EventListener* pListener : this->vecListener)
    {
        if (pListenerExcluded->getKey() != pListener->getKey())
        {
            pListener->setListenerEnabled(false);
        }
    }
}

void EventBroadcaster::enableListener(std::string strName)
{
    for (EventListener* pListener : this->vecListener)
    {
        if (pListener->getListenerOwnerName() == strName)
        {
            pListener->setListenerEnabled(true);
        }
    }
}

void EventBroadcaster::enableAllListeners()
{
    for (EventListener* pListener : this->vecListener)
    {
        pListener->setListenerEnabled(true);
    }
}

int EventBroadcaster::findListener(EventListener* pListener, std::vector<EventListener*> vecListener) {
    if(vecListener.empty())
        vecListener = this->vecListener;

    int nIndex = -1;

    for(int i = 0; i < vecListener.size() && nIndex == -1; i++) {
        if(vecListener[i] == pListener)
            nIndex = i;
    }

    return nIndex;
}

/* * * * * * * * * * * * * * * * * * * * *
 *       SINGLETON-RELATED CONTENT       *
 * * * * * * * * * * * * * * * * * * * * */
EventBroadcaster* EventBroadcaster::P_SHARED_INSTANCE = NULL;
EventBroadcaster::EventBroadcaster() {}
EventBroadcaster::EventBroadcaster(const EventBroadcaster&) {}

EventBroadcaster* EventBroadcaster::getInstance() {
    if(P_SHARED_INSTANCE == NULL)
        P_SHARED_INSTANCE = new EventBroadcaster();

    return P_SHARED_INSTANCE;
}
/* * * * * * * * * * * * * * * * * * * * */
