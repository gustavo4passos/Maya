#ifndef PLAYERDIEDEVENT_H
#define PLAYERDIEDEVENT_H

#include <memory>

#include "Event.h"

class PlayerDiedEvent : public Event {
public:
    PlayerDiedEvent()
    :   Event(EventType::PLAYER_DIED_EVENT)
    { }
};

#endif
