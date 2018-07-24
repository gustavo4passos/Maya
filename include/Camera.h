#ifndef CAMERA_H
#define CAMERA_H

#include "Vector2D.h"
#include "GameObject.h"

class Camera : public EventListener {
public:
    Camera(float fovw, float fovh, int fovleft, int fovright, int fovtop, int fovbottom, GameObject* subject = NULL);
    ~Camera();

	// Updates the camera position in relation to the subject.
	// The subject will always be at the center of the camera field of
	// view, except when that would require the camera to move beyond
	// the world limits.  
    void Update(); 
	
	// If subject == NULL, the camera will be positioned at (0, 0)
    void FocusAt(const GameObject* subject);

	// Sets the field of view and world limits for the camera
	void SetFieldOfView(int fovleft, int fovright, int fovtop, int fovbottom);

	// Zoom in and out taking the subejct as the center point
	void ZoomIn(float scale, float speed);
	void ZoomOut(float scale, float speed);

	// Camera's position 
    inline float x() const { return _pos.x(); }
    inline float y() const { return _pos.y(); }
	
	// Current zoom factor
	inline const float& zoomScale() const { return _zoomScale; }

	// Listens for LEVEL_CHANGED events, and changes the _fovw and _fovh accordingly
	virtual bool OnNotify(Event* event) override;

private:
	void FindCenter();
    const GameObject* _subject;
    Vector2D _pos;
	Vector2D _center;
	Vector2D _velocity;

    float _fovw, _fovh;
	int _fovleft, _fovright, _fovtop, _fovbottom;
	float _zoomScale;
};

#endif 
