#ifndef EVENT_H
#define EVENT_H

enum class EventType {
    // Switch events
    SWITCH_ACTIVATED,

    // Level events
    LEVEL_CHANGED,

    // Player Events
    PLAYER_HIT_TELEPORT,
    PLAYER_DIED_EVENT
};

class Event {
public:
    Event(EventType type)
    : _type(type)
    { }
    virtual ~Event() { }

    inline EventType type() const { return _type; }

    // Allows cloning an event when polymorphy is needed
    virtual Event* Clone() { return nullptr; };

private:
    EventType _type;
};

#endif
