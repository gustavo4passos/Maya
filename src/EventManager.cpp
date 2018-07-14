#include "../include/EventManager.h"

#include <string>

#include "../include/ErrorHandler.h"
#include "../include/Event.h"
#include "../include/EventListener.h"

std::map<EventType, std::set<EventListener*> > EventManager::_observers;

void EventManager::Notify(std::unique_ptr<Event>& event) {
    std::map<EventType, std::set<EventListener*>>::iterator eventListenersEntry = _observers.find(event->type());

    // Checks if there are any listeners for this event
    if(eventListenersEntry == _observers.end()){
        LOG_ERROR("There are no listeners for this event. Type: " + std::to_string((int)event->type()));
        return;
    }

    std::set<EventListener*> *eventListenersSet = &eventListenersEntry->second;
    for(std::set<EventListener*>::iterator listener = eventListenersSet->begin(); listener != eventListenersSet->end(); listener++) {
        // If current listener handles the event, do not notify the remaining listeners
        if((*listener)->OnNotify(event)) break;
    }
}

void EventManager::AddListener(EventListener* listener, EventType eventType) {
    _observers[eventType].insert(listener);
}