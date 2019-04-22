#ifndef PHYSICSENGINE_H
#define PHYSICSENGINE_H

#include "Vector2D.h"
#include "Level.h"

#include <set>
#include <utility>

enum class Kind;

enum class CollisionPosition {
    IGNORE_POSITION = 0b1,
    TOP_COLLISION = 0b10,
    BOTTOM_COLLISION = 0b11,
    LEFT_COLLISION = 0b100,
    RIGHT_COLLISION = 0b101
};

struct CollisionEvent {
    Kind kind;
    CollisionPosition collisionPosition;
    Vector2D velocity;
    int damage;
    GameObject* subject;
    
    bool operator < (const CollisionEvent& rhs) const {

        bool sameKind = kind == rhs.kind;
        bool sameVelocity = velocity == rhs.velocity;
        bool sameDamage = damage == rhs.damage;

        if (velocity < rhs.velocity) return true;
        else if (sameVelocity && damage < rhs.damage) return true;
        else if (sameVelocity && sameDamage && kind < rhs.kind) return true;
        else if (sameVelocity && sameDamage && sameKind && collisionPosition < rhs.collisionPosition) return true;
        else return false;
  }
};

class PhysicsEngine {
public:

    static void ApplyGravity(GameObject* gameObject);
    static const Vector2D& GetGravityForce() { return _gravity; }
    static void SetGravityForce(const Vector2D& strength);

    static bool CheckCollision(Rect* one, Rect* two);
    static bool OnGround(GameObject* gameObject);
	static bool OnWall(GameObject* gameObject);
    static bool OnWallLeft(GameObject* gameObject);
    static bool OnWallRight(GameObject* gameObject);
	static bool HitHead(GameObject* gameObject);
    static bool IsOnTop(Rect* bottom, Rect* top);

    inline static void setCurrentLevel(Level* currentLevel){ _currentLevel = currentLevel; }
    static void MoveAndCheckCollision(GameObject* gameObject);
    static void MoveAndCheckCollision2(GameObject* gameObject);
    static void CheckCollisionAgainstEnemies(GameObject* gameObject);
    // TEMPORARY: Until better option for testing collisions with game 
    // objects inside the debug menu is found.
    // This test is beeing used to check if the player clicked on a game object,
    // then return a pointer to this object (if any has been clicked), or nullptr
    // otherwise.
    static GameObject* CheckCollisionAgainstLevelGameObjects(Rect* rect);
    
private:
    PhysicsEngine() {}
    ~PhysicsEngine() {}
    static Level* _currentLevel;
    static void NewUnsentCollisonEvent(GameObject* gameObject1, GameObject* gameObject2);
    static bool CheckCollisionAgainstLevel(GameObject* gameObject);
    static bool CheckCollisionAgainstLevel(GameObject* gameObject, const Vector2D* furthestPosition, const Vector2D* positionToTry);
    
    static bool CheckCollisionAgainstLevelCollisionRects(Rect* rect);
    static bool CheckCollisionAgainstLevelGameObjects(GameObject* gameObject);
    static CollisionPosition CheckCollisionPosition(GameObject* gameObject1, GameObject* gameObject2);

    static Vector2D _gravity;
    static std::set<std::pair<GameObject*, CollisionEvent>> _unsentCollisionEvents;
};




#endif
