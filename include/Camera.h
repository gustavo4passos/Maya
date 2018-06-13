#ifndef CAMERA_H
#define CAMERA_H

#include "Vector2D.h"
#include "Rect.h"

class Camera {
public:
    Camera(Rect focusAt, float fovw, float fovh, int fovleft, int fovright, int fovtop, int fovbottom);
    ~Camera() { };

    void FocusAt(const Rect& center);
    void Update(); 
	void SetFieldOfView(int fovleft, int fovright, int fovtop, int fovbottom);

    float x() const { return _pos.x(); }
    float y() const { return _pos.y(); }

private:
	void FindCenter();
    Rect _focusPoint;
    Vector2D _pos;
	Vector2D _center;

    float _fovw, _fovh;
	int _fovleft, _fovright, _fovtop, _fovbottom;
};

#endif 
