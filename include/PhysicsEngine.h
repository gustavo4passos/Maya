#ifndef PHYSICSENGINE_H
#define PHYSICSENGINE_H

#include "Vector2D.h"
#include "GameEntity.h"
#include "Level.h"

class PhysicsEngine {
public:

    static void ApplyGravity(GameObject* gameObject);
    static bool CheckCollision(Rect* one, Rect* two);
    static bool OnGround(GameObject* GameObject);
    inline static void setCurrentLevel(Level* currentLevel){ _currentLevel = currentLevel; }

private:
    PhysicsEngine(){}
    ~PhysicsEngine(){}
    static Level* _currentLevel;

    static Vector2D _gravity;

};



#endif