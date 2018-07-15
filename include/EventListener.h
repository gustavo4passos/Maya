#ifndef EVENTLISTENER_H
#define EVENTLISTENER_H

#include <memory>

class Event;

class EventListener {
public:
    EventListener() { }
    virtual ~EventListener() { }
    
    // Notifiy a listener that a event just happened.
    // If the listener returns true, the event will no
    // longer be propagated to the remaining registered
    // listeners.
    virtual bool OnNotify(Event* event) = 0;
};

#endif