#ifndef EVENT_H
#define EVENT_H

enum class EventType {
    // Collision events
    PLAYER_ENEMY_COLLIDED,
    ENEMY_COLLIDED,

    // Switch events
    SWITCH_ACTIVATED,

    // Level events
    LEVEL_CHANGED
};

class Event {
public:
    Event(EventType type)
    : _type(type) 
    { }
    virtual ~Event() { }

    inline EventType type() const { return _type; }

private:
    EventType _type;
};

#endif