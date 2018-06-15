#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "Rect.h"

class Renderer; 

class GameObject {
public:
	GameObject(float x, float y, float w, float h) :
		_position(x, y),
		_velocity(0.f, 0.f),
		_w(w),
		_h(h),
		_collisionRect(x, y, w, h),
		_speed(2.5f),
		_impulse(8.f),
		_movingleft(false),
		_movingright(false)
		{ }

	inline const Rect& collisionRect() { return _collisionRect; }
	inline const Vector2D& position()  { return _position; }
	inline const Vector2D& velocity()  { return _velocity; }
	inline const float x() const { return _position.x(); }
	inline const float y() const { return _position.y(); }
	inline const float w() const { return _w; }
	inline const float h() const { return _h; }

	void setPosition(float x, float y) {
		_position.setX(x);
		_position.setY(y);
		_collisionRect.setPosition(x, y);
	}

	void setVelocity(float x, float y) {
		_velocity.setX(x);
		_velocity.setY(y);
	}

	void HandleInput();
	void Update();
	void Draw(Renderer* renderer, float positionInterpolation);

	friend class InfoMenuGL3;
	
private:
	Vector2D _position;
	Vector2D _velocity;
	float _w, _h;
	Rect _collisionRect;

	float _speed;
	float _impulse;
	bool _movingleft, _movingright;
};
#endif
