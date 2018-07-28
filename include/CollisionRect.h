#ifndef COLLISIONRECT_H
#define COLLISIONRECT_H

#include "Rect.h"

enum class CollisionBehavior {
    IGNORE = 0b1,
    BLOCK = 0b10,
    ONE_WAY = 0b100,
    BLOCK_LEFT = 0b1000,
    BLOCK_RIGHT = 0b10000,
    BLOCK_TOP = 0b100000,
    BLOCK_BOTTOM = 0b1000000
};

class CollisionRect : public Rect {
public:
    CollisionRect(float x, float y, int w, int h, float offsetX = 0, float offsetY = 0);
    CollisionRect(const Rect& bounds, CollisionBehavior collisionBehavior = CollisionBehavior::IGNORE, float offsetX = 0, float offsetY = 0);
    CollisionRect(const CollisionRect& c);

    inline float originX() const { return _position.x() - _offsetX; }
    inline float originY() const { return _position.y() - _offsetY; }
    inline float offsetX() const { return _offsetX; }
    inline float offsetY() const { return _offsetY; }
    
    inline CollisionBehavior collisionBehavior() const { return _collisionBehavior; }
    
    void setOffset(float x, float y);
    void setOffsetX(float x);
    void setOffsetY(float y);

    void setCollisionBehavior(CollisionBehavior collisionBehavior) { _collisionBehavior = collisionBehavior; }

private:
    CollisionBehavior _collisionBehavior;
    float _offsetX, _offsetY;
};

#endif