#include "../include/GameObject.h"

#include "../include/PhysicsEngine.h"
#include "../include/Renderer.h"
#include "../include/ResourceManager.h"
#include "../include/InputModule.h"

void GameObject::Update() {
	if(_movingright) _velocity.setX(_speed);
	else if(_movingleft) _velocity.setX(-_speed);
	else _velocity.setX(0);

	PhysicsEngine::ApplyGravity(this);
	PhysicsEngine::MoveAndCheckCollision(this);
	PhysicsEngine::CheckCollisionAgainstEnemies(this);

	if(PhysicsEngine::OnGround(this)){
		_velocity.setY(0.f);
	}
	if(PhysicsEngine::HitHead(this)){
	  	_velocity.setY(_velocity.y() * 0.3f);
	}
	if(PhysicsEngine::OnWall(this)){
		_velocity.setX(0.f);
		if(_velocity.y() > 0.f){
			_velocity.setY(_velocity.y() * 0.4f);
		}
	}
}

void GameObject::HandleInput() {
	if(InputModule::IsKeyPressed(InputModule::LEFT)){
		_movingleft = true;
		_movingright = false;
	}
	if(InputModule::WasKeyReleased(InputModule::LEFT)){
		_movingleft = false;
	}
	if(InputModule::IsKeyPressed(InputModule::RIGHT)){
		_movingright = true;	
		_movingleft = false;
	}
	if(InputModule::WasKeyReleased(InputModule::RIGHT)){
		_movingright = false;
	}
	if(InputModule::IsKeyPressed(InputModule::RIGHT) && InputModule::IsKeyPressed(InputModule::LEFT)){
		_movingleft = false;
		_movingright = false;
	}
	if(InputModule::WasKeyPressed(InputModule::SPACE)){
		if(PhysicsEngine::OnGround(this)){
			_velocity.setY(-_impulse);
		}
		else if(PhysicsEngine::OnWall(this)){
			_velocity.setY(-_impulse * 0.8f);
		}
	}
}

void GameObject::Draw(Renderer* renderer, float positionInterpolation) {
    Rect src = Rect(_position.x() + (_velocity.x() * positionInterpolation), _position.y() + (_velocity.y() * positionInterpolation), 
     _w,  _h);

	Rect rect = Rect(0, 0, _w, _h);

	if(_velocity.x() != 0.f || _velocity.y() != 0.f){
		int frame = (SDL_GetTicks() / 80) % 8;
		rect.setX(rect.x() + frame * _w);

		renderer->Draw(ResourceManager::GetTexture("maya_running"), &rect, &src);
	}
	else {
	 	renderer->Draw(ResourceManager::GetTexture("maya_standing"), &rect, &src);
	}
}
