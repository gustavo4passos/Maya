#include "../include/Weapon.h"

Weapon::Weapon(float x, float y, int w, int h) : GameObject(x,y,w,h){
    _kind = Kind::WEAPON;
}
Weapon::Weapon(const CollisionRect& collisionRect, int spriteW, int spriteH) : GameObject(collisionRect, spriteW, spriteH) {}
Weapon::~Weapon() { }
