#ifndef EVENTMANAGER_H
#define EVENTMANAGER_H

#include <memory>
#include <map>
#include <set>

#include "Event.h"

class EventListener;

class EventManager {
public:
    static void Notify(std::unique_ptr<Event>& event);

    static void AddListener(EventListener* listener, EventType eventType);
    static void RemoveObsever(EventListener* listener, EventType eventType);

private:
    EventManager() { }

    static std::map<EventType, std::set<EventListener*> > _observers;
};

#endif