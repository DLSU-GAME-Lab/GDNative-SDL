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
};


#endif