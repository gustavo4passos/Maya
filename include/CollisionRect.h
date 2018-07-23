#ifndef COLLISIONRECT_H
#define COLLISIONRECT_H

#include "Rect.h"

enum class CollisionBehavior {
    IGNORE = 0b1,
    BLOCK = 0b01,
    ONE_WAY = 0b001,
    BLOCK_LEFT = 0b0001,
    BLOCK_RIGHT = 0b00001,
    BLOCK_TOP = 0b000001,
    BLOCK_BOTTOM = 0b0000001
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