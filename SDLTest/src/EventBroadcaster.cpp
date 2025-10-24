#include "EventBroadcaster.h"
#include "iostream"
void EventBroadcaster::initialize()
{
    if (P_SHARED_INSTANCE == NULL)
        P_SHARED_INSTANCE = new EventBroadcaster();
}
void EventBroadcaster::destroy()
{
    delete P_SHARED_INSTANCE;
}
void EventBroadcaster::registerListener(EventListener* pListener) {
    EventKey EKey = pListener->getKey();
    this->mapListener[EKey].push_back(pListener);
    this->vecListener.push_back(pListener);
    std::cout << this->vecListener.size() << std::endl;;
}

void EventBroadcaster::unregisterListener(EventListener* pListener) {
    EventKey EKey = pListener->getKey();
    switch (EKey)
    {
    case EventKey::COLOR_MATCH:
        std::cout << "Color Match" << std::endl;
        break;
    case EventKey::COUNTER_SUBTRACT:
        std::cout << "Counter Sub" << std::endl;
        break;
    case EventKey::GAME_END:
        std::cout << "Game End" << std::endl;
        break;
    }
    std::vector<EventListener*> vecListener = this->mapListener[EKey];
    int nIndex = this->findListener(pListener, vecListener);

    this->mapListener[EKey].erase(this->mapListener[EKey].begin() + nIndex);

    nIndex = this->findListener(pListener);
    std::cout << nIndex << std::endl;
    if(nIndex != -1) {
        this->vecListener.erase(this->vecListener.begin() + nIndex);
    }
}

void EventBroadcaster::unregisterAllListeners() {
    for(EventListener* pListener : this->vecListener)
    {
 
        this->unregisterListener(pListener);
    }

    this->vecListener.clear();
    this->mapListener.clear();
}

void EventBroadcaster::broadcast(EventKey EKey, std::unordered_map<std::string, void*> mapParameter) {
    for(int i = 0; i < this->mapListener[EKey].size(); i++) {
        this->mapListener[EKey][i]->onEventTrigger(mapParameter);
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

    return P_SHARED_INSTANCE;
}
/* * * * * * * * * * * * * * * * * * * * */
