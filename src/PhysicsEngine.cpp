#include "../include/PhysicsEngine.h"

Vector2D PhysicsEngine::_gravity = Vector2D(0, 0.05); 
Level* PhysicsEngine::_currentLevel = NULL;

void PhysicsEngine::ApplyGravity(GameObject* gameObject){
    Vector2D newVelocity =  gameObject->velocity() + _gravity;
    gameObject->setVelocity( newVelocity.x() , newVelocity.y());
}

bool PhysicsEngine::CheckCollision(Rect* one, Rect* two){
    if(one->left() > two->right()) return false;
    if(one->right() < two->left()) return false;
    if(one->top() > two->bottom()) return false;
    if(one->bottom() < two->top()) return false;
    
    return true;
}
bool PhysicsEngine::OnGround(GameObject* gameObject){
    if(!_currentLevel){
        LOG_ERROR("_currentLevel in PhysicsEngine is NULL. (Forgot to call PhysicsEngine::SetCurrentLevel(Level* level)?)");
        DEBUG_BREAK();
    }
    Rect collisionObject = gameObject->collisionRect();
    for( std::vector<Rect*>::iterator it= _currentLevel->collisionRects().begin(); it!=_currentLevel->collisionRects().end(); it++){
        if(CheckCollision(&collisionObject, *it)){
            return true;
        }
    }
    return false;
}