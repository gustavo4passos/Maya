#include "../include/GameObject.h"

#include "../include/PhysicsEngine.h"
#include "../include/Renderer.h"
#include "../include/ResourceManager.h"
#include "../include/InputModule.h"

void GameObject::Update() {
	if(!PhysicsEngine::OnGround(this)){
		PhysicsEngine::ApplyGravity(this);
	}
	else {
		if(_velocity.y() > 0){
			_velocity.setY(0.f);
		}
	}
	_position += _velocity;
	_collisionRect.setPosition(_position);
}

void GameObject::HandleInput() {
	float speed = 4.f;
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
	if(InputModule::WasKeyPressed(InputModule::SPACE) && PhysicsEngine::OnGround(this)){
		_velocity.setY(-4.f);
	}
}

void GameObject::Draw(Renderer* renderer, float positionInterpolation) {

    Rect rect = Rect(0, 0, 32, 32);

    Rect src = Rect(_collisionRect.x() + (_velocity.x() * positionInterpolation), _collisionRect.y() + (_velocity.y() * positionInterpolation), 
     _collisionRect.w(),  _collisionRect.h());

    renderer->Draw(ResourceManager::GetTexture("grass"), &rect, &src);
}
