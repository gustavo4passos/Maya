#include "../include/GameObject.h"

#include "../include/Renderer.h"
#include "../include/ResourceManager.h"
#include "../include/InputModule.h"
#include "../include/PhysicsEngine.h"

void GameObject::Update() {
	PhysicsEngine::ApplyGravity(this);
	if(PhysicsEngine::OnGround(this)){
		if(_velocity.y()>0)
			_velocity.setY(0);
	}
    _position += _velocity;
    _collisionRect.setPosition(_position);
}

void GameObject::HandleInput() {
    float speed = 2.f;
    if(InputModule::WasKeyPressed(InputModule::LEFT)){
        _velocity.setX(_velocity.x() - speed);
    }
    if(InputModule::WasKeyReleased(InputModule::LEFT)){
        _velocity.setX(_velocity.x() + speed);
    }
    if(InputModule::WasKeyPressed(InputModule::RIGHT)){
        _velocity.setX(_velocity.x() + speed);
    }
    if(InputModule::WasKeyReleased(InputModule::RIGHT)){
        _velocity.setX(_velocity.x() - speed);
    }
}

void GameObject::Draw(Renderer* renderer, float positionInterpolation) {

    Rect rect = Rect(0, 0, 32, 32);

    Rect src = Rect(_collisionRect.x() + (_velocity.x() * positionInterpolation), _collisionRect.y() + (_velocity.y() * positionInterpolation), 
     _collisionRect.w(),  _collisionRect.h());

    renderer->Draw(ResourceManager::GetTexture("grass"), &rect, &src);
}