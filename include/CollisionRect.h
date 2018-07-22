#ifndef COLLISIONRECT_H
#define COLLISIONRECT_H

#include "Rect.h"

class CollisionRect : public Rect {
public:
    CollisionRect(float x, float y, int w, int h, float offsetX = 0, float offsetY = 0);
    CollisionRect(const CollisionRect& c);

    inline float originX() { return _position.x() - _offsetX; }
    inline float originY() { return _position.y() - _offsetY; }
    inline float offsetX() const { return _offsetX; }
    inline float offsetY() const { return _offsetY; }
    
    void setOffset(float x, float y);
    void setOffsetX(float x);
    void setOffsetY(float y);

private:
    float _offsetX, _offsetY;
};

#endif