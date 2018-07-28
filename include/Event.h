#ifndef EVENT_H
#define EVENT_H

enum class EventType {
    // Collision events
    PLAYER_ENEMY_COLLIDED,
    ENEMY_COLLIDED,

    // Switch events
    SWITCH_ACTIVATED,

    // Level events
    LEVEL_CHANGED,
    PLAYER_HIT_TELEPORT
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