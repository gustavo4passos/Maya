#ifndef ZONE_H
#define ZONE_H

#include "GameObject.h"

class Zone : public GameObject {

public:
    Zone(const Rect& bounds) 
    :   GameObject(CollisionRect(bounds, CollisionBehavior::PASSH_THROUGH), 0, 0)
    { }
    virtual ~Zone() {}
    virtual void Activate() = 0;
    inline bool active(){ return _active; }
    inline bool onTouch(){ return _onTouch; }

private:
    bool _active;
    bool _onTouch;

};

#endif