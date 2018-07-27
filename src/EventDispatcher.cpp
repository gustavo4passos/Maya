#include "../include/EventDispatcher.h"

#include <string>

#include "../include/ErrorHandler.h"
#include "../include/Event.h"
#include "../include/EventListener.h"

std::map<EventType, std::set<EventListener*> > EventDispatcher::_listeners;

void EventDispatcher::Notify(Event* event) {
    std::map<EventType, std::set<EventListener*>>::iterator eventListenersEntry = _listeners.find(event->type());

    // Checks if there are any listeners for this event
    if(eventListenersEntry == _listeners.end()){
        LOG_WARNING("There are no listeners for this event: Type " + std::to_string((int)event->type()));
        return;
    }
    
    std::set<EventListener*> *eventListenersSet = &eventListenersEntry->second;
    for(std::set<EventListener*>::iterator listener = eventListenersSet->begin(); listener != eventListenersSet->end(); listener++) {
        // If current listener handles the event, do not notify the remaining listeners
        if((*listener)->OnNotify(event)) break;
    }
}

void EventDispatcher::AddListener(EventListener* listener, EventType eventType) {
    _listeners[eventType].insert(listener);
}

void EventDispatcher::RemoveListener(EventListener* listener, EventType eventType) {
    std::map<EventType, std::set<EventListener*>>::iterator eventListenersEntry = _listeners.find(eventType);
    if(eventListenersEntry == _listeners.end()) {
        LOG_ERROR("Unable to remove listener: there are no listeners for this type of event. Event Type: " + std::to_string((int)eventType));
        return;
    }

    std::set<EventListener*> *listenersSet = &eventListenersEntry->second;
    std::set<EventListener*>::iterator listenerEntry = listenersSet->find(listener);
    if(listenerEntry == listenersSet->end()){
        LOG_ERROR("Unable to remove listener. Listener not in the list.");
        return;
    }

    listenersSet->erase(listenerEntry);
}