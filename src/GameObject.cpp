#include "../include/GameObject.h"

#include "../include/Event.h"
#include "../include/PhysicsEngine.h"
#include "../include/Renderer.h"
#include "../include/ResourceManager.h"
#include "../include/InputModule.h"

void GameObject::Update() {
	if(_movingright) _velocity.setX(_speed);
	else if(_movingleft) _velocity.setX(-_speed);
	else _velocity.setX(0);

	PhysicsEngine::ApplyGravity(this);
	PhysicsEngine::CheckCollisionAgainstEnemies(this);
	PhysicsEngine::MoveAndCheckCollision(this);
	
	if(PhysicsEngine::OnGround(this)){
		_velocity.setY(0.f);
	}
	if(PhysicsEngine::HitHead(this)){
	  	_velocity.setY(_velocity.y() * 0.3f);
	}
	if(PhysicsEngine::OnWall(this)){
		if(_movingright) _facingright = true;
		if(_movingleft) _facingright = false;
		_velocity.setX(0.f);
	}
}

void GameObject::HandleInput() {
	if(InputModule::IsKeyPressed(InputModule::LEFT)){
		_movingleft = true;
		_movingright = false;
	}
	if(InputModule::WasKeyReleased(InputModule::LEFT)){
		if(_movingleft) _facingright = false;
		_movingleft = false;
	}
	if(InputModule::IsKeyPressed(InputModule::RIGHT)){
		_movingright = true;	
		_movingleft = false;
	}
	if(InputModule::WasKeyReleased(InputModule::RIGHT)){
		if(_movingright) _facingright = true;
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
    Rect dst = Rect(_position.x() + (_velocity.x() * positionInterpolation), _position.y() + (_velocity.y() * positionInterpolation), 
     _w,  _h);

	Rect src = Rect(0, 0, _w, _h);

	if(_velocity.x() != 0.f || _velocity.y() != 0.f){
		int frame = (SDL_GetTicks() / 80) % 8;
		src.setX(src.x() + frame * _w);

		if(_movingright) renderer->Draw(ResourceManager::GetTexture("maya_running"), &src, &dst);
		else renderer->Draw(ResourceManager::GetTexture("maya_running"), &src, &dst, true);
	}
	else {
	 	if(_facingright) renderer->Draw(ResourceManager::GetTexture("maya_standing"), &src, &dst);
		else renderer->Draw(ResourceManager::GetTexture("maya_standing"), &src, &dst, true);
	}
}

bool GameObject::OnNotify(std::unique_ptr<Event>& event){
	if(event->type() == EventType::PLAYER_ENEMY_COLLISION) {
		_velocity.setX(0.f);
		_velocity += Vector2D(10.f, -10.f);
	}
	return false;
}  