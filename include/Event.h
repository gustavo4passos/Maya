#ifndef EVENT_H
#define EVENT_H

enum class EventType {
    // Collision events
    PLAYER_ENEMY_COLLISION,
    ENEMY_COLLISION,

    // Switch events
    ACTIVATE_SWITCH
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