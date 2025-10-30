#ifndef INTERFACES_EVENT_LISTENER_HPP
#define INTERFACES_EVENT_LISTENER_HPP

#include "EnumEventKey.h"
#include "unordered_map"
#include "string"

class EventListener {
    public:
        virtual ~EventListener() {}

    public:
        virtual void onEventTrigger(std::unordered_map<std::string, void*> mapParameter) = 0;
        virtual EventKey getKey() = 0;
        virtual bool isListenerEnabled() = 0;
        virtual void setListenerEnabled(bool bListenerEnabled) = 0;
        virtual std::string getListenerOwnerName() = 0;
};


#endif