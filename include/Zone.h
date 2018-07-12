#ifndef ZONE_H
#define ZONE_H

#include "GameObject.h"

class Zone : public GameObject{

public:
    Zone(float x, float y, float w, float h) : GameObject(x, y, w, h) {}
    virtual ~Zone(){}
    virtual void Activate() = 0;
    inline bool active(){ return _active; }
    inline bool onTouch(){ return _onTouch; }

private:
    bool _active;
    bool _onTouch;

};

#endif