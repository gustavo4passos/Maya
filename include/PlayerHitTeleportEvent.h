#ifndef PLAYERHITTELEPORTEVENT_H
#define PLAYERHITTELEPORTEVENT_H

#include <string>

#include "Event.h"
#include "Vector2D.h"

class PlayerHitTeleportEvent : public Event {
public:
    PlayerHitTeleportEvent(const std::string& destinationLevel, Vector2D destinationPosition)
    :   Event(EventType::PLAYER_HIT_TELEPORT),
        _destinationLevel(destinationLevel),
        _destinationPosition(destinationPosition)
    { }

    PlayerHitTeleportEvent(const PlayerHitTeleportEvent& e)
    :   PlayerHitTeleportEvent(e._destinationLevel, e._destinationPosition)
    { }

    ~PlayerHitTeleportEvent() { }

    inline const std::string& destinationLevel() const { return _destinationLevel; };
    inline const Vector2D destinationPosition() const { return _destinationPosition; }

    virtual Event* Clone() { return new PlayerHitTeleportEvent(*this); }

private:
    std::string _destinationLevel;
    Vector2D _destinationPosition;
};

#endif
