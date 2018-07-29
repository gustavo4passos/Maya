#include "../include/TeleportZone.h"

#include <memory> // Smart pointers

#include "../include/EventDispatcher.h"
#include "../include/PlayerHitTeleportEvent.h"


TeleportZone::TeleportZone(const Rect& bounds, const std::string& destinationLevel, const Vector2D& destinationPosition)
:   Zone(bounds),
    _destinationLevel(destinationLevel),
    _destinationPosition(destinationPosition)
{ }

void TeleportZone::Update() {
    while(!_unresolvedCollisionEvents.empty()) {
        CollisionEvent collisionEvent = _unresolvedCollisionEvents.front();
        _unresolvedCollisionEvents.pop();

        // Notify the EventDispatcher system that the player hit a teleport zone
        if(collisionEvent.collisionEventType == CollisionEventType::EVENT_COLLISION) {
            std::unique_ptr<Event> teleportCollisionEvent(new PlayerHitTeleportEvent(_destinationLevel, _destinationPosition));
            EventDispatcher::Notify(teleportCollisionEvent.get());
        }
    }
}
