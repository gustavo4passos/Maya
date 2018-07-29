#ifndef TELEPORTZONE_H
#define TELEPORTZONE_H

#include "Vector2D.h"
#include "Zone.h"

class TeleportZone : public Zone {
public: 
    TeleportZone(const Rect& bounds, const std::string& destinationLevel, const Vector2D& destinationPosition);
    virtual ~TeleportZone() { }
    
    virtual void Update();

    virtual void Activate() { }
    
private:
    std::string _destinationLevel;
    Vector2D _destinationPosition;
};

#endif