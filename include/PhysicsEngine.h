#ifndef PHYSICSENGINE_H
#define PHYSICSENGINE_H

#include "Vector2D.h"
#include "GameEntity.h"
#include "Level.h"

class PhysicsEngine {
public:

    static void ApplyGravity(GameObject* gameObject);
    static bool CheckCollision(Rect* one, Rect* two);
    static bool OnGround(GameObject* gameObject);
    inline static void setCurrentLevel(Level* currentLevel){ _currentLevel = currentLevel; }
    static void MoveAndCheckCollision(GameObject* gameObject);

private:
    PhysicsEngine(){}
    ~PhysicsEngine(){}
    static Level* _currentLevel;
    static bool CheckCollisionAgainstLevel(Rect* rect);
    static Vector2D _gravity;

};



#endif