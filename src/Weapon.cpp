#include "../include/Weapon.h"

Weapon::Weapon(float x, float y, int w, int h) : GameObject(x, y, w, h) {
    _kind = Kind::WEAPON;
    _damage = 1;
}
Weapon::Weapon(const CollisionRect& collisionRect, int spriteW, int spriteH) : GameObject(collisionRect, spriteW, spriteH) {
    _kind = Kind::WEAPON;
    _damage = 1;
}
Weapon::~Weapon() {}