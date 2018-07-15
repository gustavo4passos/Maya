#ifndef PHYSICSENGINE_H
#define PHYSICSENGINE_H

#include "Vector2D.h"
#include "Level.h"

enum class CollisionEventType {
    ENEMY_COLLISION,
    EVENT_COLLISION,
    PLAYER_ENEMY_COLLIDED
};

struct CollisionEvent {
    GameObject* gameObject;
    CollisionEventType collisionEventType;
    Vector2D velocity;
    int damage;
};

class PhysicsEngine {
public:

    static void ApplyGravity(GameObject* gameObject);
    static bool CheckCollision(Rect* one, Rect* two);
    static bool OnGround(GameObject* gameObject);
	static bool OnWall(GameObject* gameObject);
	static bool HitHead(GameObject* gameObject);

    inline static void setCurrentLevel(Level* currentLevel){ _currentLevel = currentLevel; }
    static void MoveAndCheckCollision(GameObject* gameObject);
	static Vector2D MoveAndCheckCollision(Rect* origin, Rect* destination);
    static void CheckCollisionAgainstEnemies(GameObject* gameObject);

	friend class InfoMenuGL3;
private:

    PhysicsEngine(){}
    ~PhysicsEngine(){}
    static Level* _currentLevel;
    static bool CheckCollisionAgainstLevel(Rect* rect);
    static Vector2D _gravity;
};




#endif
