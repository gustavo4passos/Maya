#include "../include/MovingPlatform.h"

#include "../include/GameSwitches.h"
#include "../include/Renderer.h"
#include "../include/ResourceManager.h"
#include "../include/ServiceLocator.h"

MovingPlatform::MovingPlatform(Vector2D origin, Vector2D destination, bool loops, const std::string& switchRequired) 
:   GameObject(CollisionRect(Rect(origin.x(), origin.y(), 45, 11), CollisionBehavior::BLOCK, 0, 2), 45, 13),
    _origin(origin),
    _destination(destination),
    _loops(loops),
    _switchRequired(switchRequired),
    _reachedDestination(false)
{
    if(_switchRequired == "") _on = true;
    _textureName = "moving-platform";
    _kind = Kind::MOVING_PLATFORM;
}

void MovingPlatform::Move() {
    if(!_reachedDestination) {
        if(x() < _destination.x()) {
            setPosition(x() + _velocity.x(), y());
            _displacement = _velocity;
        }
        else {
            setPosition(x() - _velocity.x(), y());
            _displacement = -_velocity;
        }
        if((int)_destination.x() == (int)x()) {
            _reachedDestination = true;
        }
    }
    else if(_loops) {
            if(x() < _origin.x()) {
            setPosition(x() + _velocity.x(), y());
            _displacement = _velocity;
        }
        else {
            setPosition(x() - _velocity.x(), y());
            _displacement = -_velocity;
        }
        if((int)_origin.x() == (int)x()) {
            _reachedDestination = false;
        } 
    }

}

void MovingPlatform::Update() {
    if(_switchRequired != "") {
        if(ServiceLocator::GetGameSwitches()->CheckSwitch(_switchRequired)) {
            _on = true;
            _velocity.setX(0.8f);
            
        }
        else {
            _on = false;
            _velocity.setX(0.0f);

        }
    }
    
    if(_on) Move();


    std::set<GameObject*> objectsOnTop;

    while(!_unresolvedCollisionEvents.empty()) {
        CollisionEvent collision = _unresolvedCollisionEvents.front();
        objectsOnTop.insert(collision.subject);
        _unresolvedCollisionEvents.pop();
    }

    for(auto object = objectsOnTop.begin(); object != objectsOnTop.end(); object++) {
        (*object)->setPosition((*object)->x() + _displacement.x(), (*object)->y());
    }
}

void MovingPlatform::Draw(Renderer* renderer, float deltaTime) {
    Rect src = Rect(0.f, 0.f, _spriteW, _spriteH);
    if(_on) {
        src.setX(_spriteW);
    }

    Rect dst = Rect(_collisionRect.originX() + _velocity.x() * deltaTime, _collisionRect.originY() + _velocity.y() * deltaTime, _spriteW, _spriteH);

    renderer->Draw(ResourceManager::GetTexture(_textureName), &src, &dst);
}