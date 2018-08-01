#ifndef ACTIVATESWITCHEVENT_H
#define ACTIVATESWITCHEVENT_H

#include <string>

#include "Event.h"
#include "EventDispatcher.h"

class ActivateSwitchEvent : public Event {
public:
    ActivateSwitchEvent(const std::string& switchID) 
    :   Event(EventType::SWITCH_ACTIVATED),
        _switchID(switchID)
    { }

    inline const std::string& switchID() const { return _switchID; }

private:
    std::string _switchID;
};

#endif