#include "../include/PhysicsEngine.h"

Vector2D PhysicsEngine::_gravity = Vector2D(0, 0.13); 
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
	collisionObject.setY(collisionObject.y() + 1.f);
    if(CheckCollisionAgainstLevel(&collisionObject)){
        return true;
    }
    else {
        return false;
    }
}

void PhysicsEngine::MoveAndCheckCollision(GameObject* gameObject){
    int nSteps=(int)(gameObject->velocity().Length() *2) + 1;
    Vector2D furthestPosition = gameObject->position();
    Vector2D oneStep = gameObject->velocity() / nSteps;

    for (int i = 1; i <= nSteps; i++){
        Vector2D positionToTry = gameObject->position() + (oneStep * i);
        Rect newCollider(positionToTry, gameObject->collisionRect().w(), gameObject->collisionRect().h());
        if(!CheckCollisionAgainstLevel(&newCollider)){
            furthestPosition = positionToTry;
        }
        else{
            gameObject->setPosition(furthestPosition.x(), furthestPosition.y());
            if(gameObject->velocity().x() != 0 && gameObject->velocity().y() != 0){
                Vector2D originalVelocity = gameObject->velocity();
                int stepsLeft = nSteps - (i - 1);

                gameObject->setVelocity((oneStep.x() * stepsLeft) , 0);
                MoveAndCheckCollision(gameObject);

                gameObject->setVelocity(0 , (oneStep.y() * stepsLeft));
                MoveAndCheckCollision(gameObject);

                gameObject->setVelocity(originalVelocity.x(), originalVelocity.y());
            }
            if(OnGround(gameObject)){
                gameObject->setVelocity(gameObject->velocity().x(), 0);
            }
            if(HitHead(gameObject)){
                gameObject->setVelocity(gameObject->velocity().x(), gameObject->velocity().y() * 0.3f);
            }
            return;
        }
    }
    gameObject->setPosition( furthestPosition.x(), furthestPosition.y());

}

bool PhysicsEngine::CheckCollisionAgainstLevel(Rect* rect){
    if(!_currentLevel){
        LOG_ERROR("_currentLevel in PhysicsEngine is NULL. (Forgot to call PhysicsEngine::SetCurrentLevel(Level* level)?)");
        DEBUG_BREAK();
    }
    for(std::vector<Rect*>::iterator it = _currentLevel->collisionRects().begin(); 
	    it!=_currentLevel->collisionRects().end(); it++){
        if(CheckCollision(rect, *it)){
            return true;
        }
    }
    return false;
}
bool PhysicsEngine::HitHead(GameObject* gameObject){
    if(!_currentLevel){
        LOG_ERROR("_currentLevel in PhysicsEngine is NULL. (Forgot to call PhysicsEngine::SetCurrentLevel(Level* level)?)");
        DEBUG_BREAK();
    }

    Rect collisionObject = gameObject->collisionRect();
    collisionObject.setY(collisionObject.y() - 1.f);

    if(CheckCollisionAgainstLevel(&collisionObject)){
        return true;
    }

    return false;
}
