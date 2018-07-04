#ifndef CAMERA_H
#define CAMERA_H

#include "Vector2D.h"
#include "Rect.h"

class GameObject;

class Camera {
public:
    Camera(float fovw, float fovh, int fovleft, int fovright, int fovtop, int fovbottom, GameObject* subject = NULL);
    ~Camera() { };

    void Update(); 
	
	// If subject == NULL, the camera will be positioned at (0, 0)
    void FocusAt(const GameObject* subject);
	void SetFieldOfView(int fovleft, int fovright, int fovtop, int fovbottom);
	
	void ZoomIn(float scale, float speed);
	void ZoomOut(float scale, float speed);

    float x() const { return _pos.x(); }
    float y() const { return _pos.y(); }

private:
	void FindCenter();
    const GameObject* _subject;
    Vector2D _pos;
	Vector2D _center;
	Vector2D _velocity;

    float _fovw, _fovh;
	int _fovleft, _fovright, _fovtop, _fovbottom;
};

#endif 
