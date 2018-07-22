#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <queue>

#include "EventListener.h"
#include "CollisionRect.h"
#include "PhysicsEngine.h"

class Renderer; 

class GameObject : public EventListener {
public:
	GameObject(const CollisionRect& collisionRect, int spriteW, int spriteH);
	virtual ~GameObject();

	inline const Rect& collisionRect() { return _collisionRect; }
	inline const Vector2D& position() { return _collisionRect.position(); }
	inline const Vector2D& velocity()  { return _velocity; }
	inline const float x() const { return _collisionRect.x(); }
	inline const float y() const { return _collisionRect.y(); }
	inline const float w() const { return _collisionRect.w(); }
	inline const float h() const { return _collisionRect.h(); }
	inline int const damage() { return _damage; }

	void setPosition(float x, float y);
	void setVelocity(float x, float y);

	virtual void HandleInput();
	virtual void Update();
	virtual void Draw(Renderer* renderer, float deltaTime);

	virtual bool OnNotify(Event* event);

	inline void EnqueueCollisionEvent(CollisionEvent collisionEvent) { _unresolvedCollisionEvents.push(collisionEvent); }

	// Grants access to the collision rect inside the physics engine, to avoid creating a copy at each frame
	friend class PhysicsEngine; 
	
	// (DEBUB) Grants access to speed and impulse to the debug menu
	friend class InfoMenuGL3;

protected:
	Vector2D _velocity;

	float _spriteW, _spriteH;
	CollisionRect _collisionRect;

	float _speed;
	float _impulse;
	int _damage;
	
	bool _movingleft, _movingright;
	bool  _facingright;

	std::queue<CollisionEvent> _unresolvedCollisionEvents;
};
#endif
