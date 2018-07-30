	#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <queue>

#include "EventListener.h"
#include "CollisionRect.h"
#include "PhysicsEngine.h"

class Renderer; 

enum class Kind {
	ENEMY,
	PLAYER,
	WEAPON,
	ZONE,
	MOVING_PLATFORM,
	PLATFORM_SWITCH,
	BUTTON,
	TELEPORT_ZONE
};

class GameObject : public EventListener {
public:
	GameObject(float x, float y, int w, int h);
	GameObject(const CollisionRect& collisionRect, int spriteW, int spriteH);

	virtual ~GameObject();

	inline const CollisionRect& collisionRect() { return _collisionRect; }	
	inline const Vector2D& position() { return _collisionRect.position(); }
	virtual inline const Vector2D& velocity() { return _velocity; }
	inline const float x() const { return _collisionRect.x(); }
	inline const float y() const { return _collisionRect.y(); }
	inline const float w() const { return _collisionRect.w(); }
	inline const float h() const { return _collisionRect.h(); }
	inline bool isActive() const { return _isActive; }
	inline int damage() const { return _damage; }
	inline Kind kind() const { return _kind;  }



	//TEMPORARIAAAAA
	inline CollisionRect& collisionRectCHANGEBLE() { return _collisionRect; }
	//TEMPORARIAAAA

	void setPosition(float x, float y);
	void setVelocity(float x, float y);
	
	void Activate() { _isActive = true; }
	void Deactivate() { _isActive = false; }

	virtual void Update();
	virtual void Draw(Renderer* renderer, float deltaTime);

	virtual bool OnNotify(Event* event);

	inline void EnqueueCollisionEvent(CollisionEvent collisionEvent) { _unresolvedCollisionEvents.push(collisionEvent); }

	// Grants access to the collision rect inside the physics engine, to avoid creating a copy at each frame
	friend class PhysicsEngine; 
	
	// (DEBUB) Grants access to speed and impulse to the debug menu
	friend class InfoMenuGL3;
	
	// Grants access to weapon collision rect
	friend class Maya;

protected:
	
	// Physics
	Vector2D _velocity;
	CollisionRect _collisionRect;
	float _speed;
	float _impulse;
	int _damage;

	// Flags
	bool  _facingright;
	bool _isActive;

	// Sprite
	int _currentFrame, _currentRow;
	int _numRows, _numFrames;
	int _frameTime;
	float _spriteW, _spriteH;

	std::string _textureName;

	//Events
	std::queue<CollisionEvent> _unresolvedCollisionEvents;

	// Kind
	Kind _kind;
};

#endif
