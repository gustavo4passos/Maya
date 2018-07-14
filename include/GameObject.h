#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <queue>

#include "EventListener.h"
#include "Rect.h"
#include "PhysicsEngine.h"

class Renderer; 

class GameObject : public EventListener {
public:
	GameObject(float x, float y, float w, float h, float collisionOffsetX = 12, float collisionOffsetY = 5, float collisionRectW = 10, float collisionRectH = 30)
	:	_position(x, y),
		_velocity(0.f, 0.f),
		_w(w),
		_h(h),
		_collisionRect(x, y, collisionRectW, collisionRectH),
		_speed(2.5f),
		_impulse(8.f),
		_movingleft(false),
		_movingright(false),
		_collisionOffsetX(collisionOffsetX),
		_collisionOffsetY(collisionOffsetY),
		_collisionW(collisionRectW),
		_collisionH(collisionRectH)
		{ setPosition(_position.x(), _position.y()); }

	virtual ~GameObject() { }
	inline const Rect& collisionRect() { return _collisionRect; }
	inline const Vector2D& position()  { return _position; }
	inline const Vector2D& velocity()  { return _velocity; }
	inline const float x() const { return _position.x(); }
	inline const float y() const { return _position.y(); }
	inline const float w() const { return _w; }
	inline const float h() const { return _h; }
	inline int const damage() { return _damage; }

	void setPosition(float x, float y) {
		_position.setX(x - _collisionOffsetX);
		_position.setY(y - _collisionOffsetY);
		_collisionRect.setPosition(x, y);
	}

	void setVelocity(float x, float y) {
		_velocity.setX(x);
		_velocity.setY(y);
	}

	virtual void HandleInput();
	virtual void Update();
	virtual void Draw(Renderer* renderer, float positionInterpolation);

	virtual bool OnNotify(std::unique_ptr<Event>& event);

	inline void EnqueueCollisionEvent(CollisionEvent collisionEvent) { _unresolvedCollisionEvents.push(collisionEvent); }

	friend class InfoMenuGL3;
	
	// Grants access to the collision rect inside the physics engine, to avoid creating a copy at each frame
	friend class PhysicsEngine; 
	
protected:
	Vector2D _position;
	Vector2D _velocity;

	float _w, _h;
	Rect _collisionRect;
	float _speed;
	float _impulse;
	int _damage;
	bool _movingleft, _movingright;
	bool  _facingright;

	float _collisionOffsetX, _collisionOffsetY, _collisionW, _collisionH;

	std::queue<CollisionEvent> _unresolvedCollisionEvents;
};
#endif
