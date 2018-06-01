#ifndef PHYSICSENGINE_H
#define PHYSICSENGINE_H

#include "Vector2D.h"
#include "GameEntity.h"

class PhysicsEngine {
public:

    static void ApplyGravity(GameEntity* gameEntity);
    bool CheckCollision(Rect* one, Rect* two);

    



private:
    PhysicsEngine(){}
    ~PhysicsEngine(){}

    static Vector2D _gravity;

};



#endif