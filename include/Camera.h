#ifndef CAMERA_H
#define CAMERA_H

#include "Vector2D.h"
#include "Rect.h"

class Camera {
public:
    Camera(Rect center, float fovw, float fovh);
    ~Camera() { };

    void SetCenter(Rect* center);
    void Update(); 

    float x() const { return _pos.x(); }
    float y() const { return _pos.y(); }

private:
    Rect _center;
    Vector2D _pos;

    float _fovw, _fovh;
};

#endif 