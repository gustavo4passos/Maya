#include "../include/PhysicsEngine.h"

#include <memory>

#include "../include/ErrorHandler.h"
#include "../include/EventDispatcher.h"
#include "../include/Enemy.h"
#include "../include/ActivateSwitchEvent.h"

Vector2D PhysicsEngine::_gravity = Vector2D(0, 0.4);
Level* PhysicsEngine::_currentLevel = NULL;
std::set<std::pair<GameObject*, CollisionEvent>> PhysicsEngine::_unsentCollisionEvents;

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

CollisionPosition PhysicsEngine::CheckCollisionPosition(GameObject* gameObject1, GameObject* gameObject2){
    Rect rect1 = gameObject1->collisionRect();
    Rect rect2 = gameObject2->collisionRect();

    if(rect1.top() > rect2.bottom()){
        return CollisionPosition::TOP_COLLISION;
    }
    else if(rect1.left() > rect2.right()){
        return CollisionPosition::LEFT_COLLISION;
    }
    else if(rect1.right() < rect2.left()){
        return CollisionPosition::RIGHT_COLLISION;
    }
    else if(rect1.bottom() < rect2.top()){
        return CollisionPosition::BOTTOM_COLLISION;
    }
    return CollisionPosition::IGNORE_POSITION;
}

bool PhysicsEngine::OnGround(GameObject* gameObject){
    if(!_currentLevel){
        LOG_ERROR("_currentLevel in PhysicsEngine is NULL. (Forgot to call PhysicsEngine::SetCurrentLevel(Level* level)?)");
        DEBUG_BREAK();
    }

    Vector2D collisionObject = gameObject->collisionRect().position();
    Vector2D offsetCollisionObject = collisionObject;
	  offsetCollisionObject.setY(collisionObject.y() + 1.f);
    if(CheckCollisionAgainstLevel(gameObject, &collisionObject, &offsetCollisionObject)){
        return true;
    }
    else {
        return false;
    }
}

bool PhysicsEngine::OnWall(GameObject* gameObject){
  	Rect offsetRect = gameObject->collisionRect();

	offsetRect.setPosition(offsetRect.x() + 1, offsetRect.y());
	if(CheckCollisionAgainstLevel(&offsetRect)) return true;

	offsetRect.setPosition(offsetRect.x() - 2, offsetRect.y());
	if(CheckCollisionAgainstLevel(&offsetRect)) return true;

	return false;
}

bool PhysicsEngine::OnWallRight(GameObject* gameObject){
  	Rect offsetRect = gameObject->collisionRect();

	offsetRect.setPosition(offsetRect.x() + 1, offsetRect.y());
	if(CheckCollisionAgainstLevel(&offsetRect)) return true;

	return false;
}

bool PhysicsEngine::OnWallLeft(GameObject* gameObject){
  	Rect offsetRect = gameObject->collisionRect();

	offsetRect.setPosition(offsetRect.x() - 2, offsetRect.y());
	if(CheckCollisionAgainstLevel(&offsetRect)) return true;

	return false;
}

bool PhysicsEngine::HitHead(GameObject* gameObject){
	if(!_currentLevel){
        LOG_ERROR("_currentLevel in PhysicsEngine is NULL. (Forgot to call PhysicsEngine::SetCurrentLevel(Level* level)?)");
        DEBUG_BREAK();
	}

  Vector2D collisionObject = gameObject->collisionRect().position();
  Vector2D offsetCollisionObject = collisionObject;
  offsetCollisionObject.setY(collisionObject.y() - 1.f);
  if(CheckCollisionAgainstLevel(gameObject, &collisionObject, &offsetCollisionObject)){
		return true;
	}

	return false;
}

bool PhysicsEngine::IsOnTop(Rect* bottom, Rect* top) {
    Rect topOffset = *top;
    topOffset.setY(topOffset.y() + 1);

    if(CheckCollision(bottom, &topOffset)) {
        return true;
    }
    return false;
}

void PhysicsEngine::MoveAndCheckCollision(GameObject* gameObject){

  MoveAndCheckCollision2(gameObject);
  for(auto it=_unsentCollisionEvents.begin(); it!=_unsentCollisionEvents.end(); it++){
      it->first->EnqueueCollisionEvent(it->second);
  }
  _unsentCollisionEvents.clear();
}

void PhysicsEngine::MoveAndCheckCollision2(GameObject* gameObject){
    int nSteps=(int)(gameObject->velocity().Length() *2) + 1;
    Vector2D furthestPosition = gameObject->collisionRect().position();
    Vector2D oneStep = gameObject->velocity() / nSteps;

    for (int i = 1; i <= nSteps; i++){
        Vector2D positionToTry = gameObject->collisionRect().position() + (oneStep * i);
        Rect newCollider(positionToTry, gameObject->collisionRect().w(), gameObject->collisionRect().h());
        if(!CheckCollisionAgainstLevel(gameObject, &furthestPosition, &positionToTry)){
            furthestPosition = positionToTry;
        }
        else{
            gameObject->setPosition(furthestPosition.x(), furthestPosition.y());
            if(gameObject->velocity().x() != 0 && gameObject->velocity().y() != 0){
                Vector2D originalVelocity = gameObject->velocity();
                int stepsLeft = nSteps - (i - 1);

                gameObject->setVelocity((oneStep.x() * stepsLeft) , 0);
                MoveAndCheckCollision2(gameObject);

                gameObject->setVelocity(0 , (oneStep.y() * stepsLeft));
                MoveAndCheckCollision2(gameObject);

                gameObject->setVelocity(originalVelocity.x(), originalVelocity.y());
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

    for(std::vector<CollisionRect*>::const_iterator it = _currentLevel->collisionRects().begin();
	    it!=_currentLevel->collisionRects().end(); it++){
        if(CheckCollision(rect, *it)){
            return true;
        }
    }

    for(auto gameObject = _currentLevel->gameObjects().begin(); gameObject != _currentLevel->gameObjects().end(); gameObject++) {
        if((*gameObject)->collisionRect().collisionBehavior() == CollisionBehavior::IGNORE) {
            continue;
        }

        Rect collisionRect = (*gameObject)->collisionRect();
        if(CheckCollision(rect, &collisionRect)) {
            return true;
        }
    }

    return false;
}

bool PhysicsEngine::CheckCollisionAgainstLevel(GameObject* gameObject, const Vector2D* furthestPosition,
  const Vector2D* positionToTry) {
    Rect rect(*positionToTry, gameObject->collisionRect().w(), gameObject->collisionRect().h());
    Rect previousPosition(*furthestPosition, gameObject->collisionRect().w(), gameObject->collisionRect().h());
    if(!_currentLevel){
        LOG_ERROR("_currentLevel in PhysicsEngine is NULL. (Forgot to call PhysicsEngine::SetCurrentLevel(Level* level)?)");
        DEBUG_BREAK();
    }

    for(std::vector<CollisionRect*>::const_iterator it = _currentLevel->collisionRects().begin();
	    it!=_currentLevel->collisionRects().end(); it++){
        if((*it)->collisionBehavior() == CollisionBehavior::ONE_WAY) {
            if(previousPosition.bottom() < (*it)->top()){
                if(CheckCollision(&rect, *it)) {
                    return true;
                }
            }
        }
        else if(CheckCollision(&rect, *it)){
            return true;
        }
    }

    for(auto it = _currentLevel->gameObjects().begin(); it != _currentLevel->gameObjects().end(); it++) {
        
        if(*it == gameObject) continue;

        if((*it)->collisionRect().collisionBehavior() == CollisionBehavior::IGNORE) {
            continue;
        }

        Rect collisionRect = (*it)->collisionRect();

        if((*it)->collisionRect().collisionBehavior() == CollisionBehavior::ONE_WAY) {
            if(previousPosition.bottom() < collisionRect.top()){
                if(CheckCollision(&rect, &collisionRect)) {
                    NewUnsentCollisonEvent(gameObject, *it);
                    return true;
                }
            }
        }

        else if(CheckCollision(&rect, &collisionRect)) {
            NewUnsentCollisonEvent(gameObject, *it);
            return true;
        }
    }

    return false;
}

void PhysicsEngine::NewUnsentCollisonEvent(GameObject* gameObject1, GameObject* gameObject2){

    CollisionEvent collisionEvent1 = { gameObject1->_kind, CheckCollisionPosition(gameObject2, gameObject1),
         gameObject1->velocity(), gameObject1->damage(), gameObject1 };
    _unsentCollisionEvents.insert(std::make_pair(gameObject2, collisionEvent1));

    CollisionEvent collisionEvent2 = { gameObject2->_kind, CheckCollisionPosition(gameObject1, gameObject2),
         gameObject2->velocity(), gameObject2->damage(), gameObject2 };
    _unsentCollisionEvents.insert(std::make_pair(gameObject1, collisionEvent2));
}

void PhysicsEngine::CheckCollisionAgainstEnemies(GameObject* gameObject){
    if(!_currentLevel){
        LOG_ERROR("_currentLevel in PhysicsEngine is NULL. (Forgot to call PhysicsEngine::SetCurrentLevel(Level* level)?)");
        DEBUG_BREAK();
    }

    for(std::vector<Enemy*>::const_iterator it = _currentLevel->enemies().begin();
        it != _currentLevel->enemies().end(); it++)
    {
        if(CheckCollision(&gameObject->_collisionRect, &(*it)->_collisionRect)){
            CollisionEvent enemyCollisionEvent = { (*it)->_kind, CheckCollisionPosition(gameObject, *it),
                 (*it)->velocity(), (*it)->damage() };
            gameObject->EnqueueCollisionEvent(enemyCollisionEvent);
            //Como sera determinado o dano da maya em relacao aos diferentes estados e equipamentos ?
            CollisionEvent mayaCollisionEvent = { gameObject->_kind, CheckCollisionPosition(*it, gameObject),
                 gameObject->velocity(), gameObject->damage()};
            (*it)->EnqueueCollisionEvent(mayaCollisionEvent);

            // std::unique_ptr<Event> playerCollision(new PlayerCollisionEvent(100, Vector2D(0, 0), Vector2D(0, 0)));
            // EventDispatcher::Notify(playerCollision.get());
        }
    }
}
