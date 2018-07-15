#ifndef EVENTMANAGER_H
#define EVENTMANAGER_H

#include <map>
#include <set>

#include "Event.h"

class EventListener;

class EventDispatcher {
public:
    static void Notify(Event* event);

    static void AddListener(EventListener* listener, EventType eventType);
    static void RemoveListener(EventListener* listener, EventType eventType);

private:
    EventDispatcher() { }

    static std::map<EventType, std::set<EventListener*> > _listeners;
};

#endif