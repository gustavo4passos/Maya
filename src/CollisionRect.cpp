#include "../include/CollisionRect.h"

CollisionRect::CollisionRect(float x, float y, int w, int h, float offsetX, float offsetY)
:    Rect(x, y, w, h),
    _offsetX(offsetX),
    _offsetY(offsetY)
{ }    

CollisionRect::CollisionRect(const CollisionRect& c) 
: CollisionRect(c.x(), c.y(), c.w(), c.h(), c._offsetX, c._offsetY)
{ }

void CollisionRect::setOffset(float offsetX, float offsetY) {
    _offsetX = offsetX;
    _offsetY = offsetY;
}

void CollisionRect::setOffsetX(float newOffsetX) {
    _offsetX = newOffsetX;
}   
void CollisionRect::setOffsetY(float newOffsetY) {
    _offsetY = newOffsetY;
}