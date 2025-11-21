#ifndef SYSTEMS_EVENT_BROADCASTER_HPP
#define SYSTEMS_EVENT_BROADCASTER_HPP

#include "EventListener.h"
#include "unordered_map"
#include "EnumEventKey.h"
#include "string"


class EventBroadcaster {
    private:
        std::unordered_map<EventKey, std::vector<EventListener*>> mapListener;
        std::vector<EventListener*> vecListener;
        
    public:
        static void initialize();
        static void destroy();
        void registerListener(EventListener* pListener);
        void unregisterListener(EventListener* pListener);
        void unregisterAllListeners();
        void broadcast(EventKey EKey, std::unordered_map<std::string, void*> mapParameter = {});
        void disableOtherListenerExcept(EventListener* pListenerExcluded);
        void enableListener(std::string strName);
        void enableAllListeners();

    private:
        int findListener(EventListener* pListener, std::vector<EventListener*> vecListener = {});

    /* * * * * * * * * * * * * * * * * * * * * 
        *       SINGLETON-RELATED CONTENT       * 
        * * * * * * * * * * * * * * * * * * * * */
    private:
        static EventBroadcaster* P_SHARED_INSTANCE;

    private:
        EventBroadcaster();
        EventBroadcaster(const EventBroadcaster&);
        EventBroadcaster& operator = (const EventBroadcaster&);

    public:
        static EventBroadcaster* getInstance();
    /* * * * * * * * * * * * * * * * * * * * */
};


#endif