#include "../include/Camera.h"

Camera::Camera(Rect center, float fovw, float fovh) : 
    _center(center),
    _pos(_center.x(), _center.y()),
    _fovw(fovw),
    _fovh(fovh)
{ }

void Camera::SetCenter(Rect* center) {
    _center = *(center);
}

void Camera::Update(){
    _pos.setX(((_fovw - _center.w()) / 2) - _center.x());
    _pos.setY(((_fovh - _center.h()) / 2) - _center.y());
}

