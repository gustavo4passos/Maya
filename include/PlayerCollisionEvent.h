#ifndef PLAYERCOLLISIONEVENT_H
#define PLAYERCOLLISIONEVENT_H

#include "Event.h"
#include "EventManager.h"
#include "Vector2D.h"

class PlayerCollisionEvent : public Event {
public:
    PlayerCollisionEvent(int damage, Vector2D position, Vector2D velocity)
    : Event(EventType::PLAYER_ENEMY_COLLISION),
      _damage(damage),
      _position(position),
      _velocity(velocity)
    { }

private:
    int _damage;
    Vector2D _position;
    Vector2D _velocity;
};

#endif