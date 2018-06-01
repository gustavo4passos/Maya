#include "../include/PhysicsEngine.h"

Vector2D PhysicsEngine::_gravity = Vector2D(0, 0.05); 

void PhysicsEngine::ApplyGravity(GameEntity* gameEntity){
    gameEntity->velocity() =  gameEntity->velocity() + _gravity;
}

bool PhysicsEngine::CheckCollision(Rect* one, Rect* two){
    if(one->left() > two->right()) return false;
    if(one->right() < two->left()) return false;
    if(one->top() > two->bottom()) return false;
    if(one->bottom() < two->top()) return false;
    
    return true;
}