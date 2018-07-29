#ifndef WEAPON_H
#define WEAPON_H

#include "GameObject.h"

class Weapon : public GameObject {
public:

    Weapon(float x, float y, int w, int h);
    Weapon(const CollisionRect& collisionRect, int spriteW, int spriteH);
    ~Weapon();

};

#endif