#include "../include/GameObject.h"

#include "../include/Event.h"
#include "../include/EventDispatcher.h"
#include "../include/InputModule.h"
#include "../include/PhysicsEngine.h"
#include "../include/Renderer.h"
#include "../include/ResourceManager.h"

GameObject::GameObject(float x, float y, float w, float h, float collisionOffsetX, float collisionOffsetY, float collisionRectW, float collisionRectH)
:	_position(x, y),
	_velocity(0.f, 0.f),
	_w(w),
	_h(h),
	_collisionRect(x, y, collisionRectW, collisionRectH),
	_speed(2.5f),
	_impulse(8.f),
	_movingleft(false),
	_movingright(false),
	_collisionOffsetX(collisionOffsetX),
	_collisionOffsetY(collisionOffsetY),
	_collisionW(collisionRectW),
	_collisionH(collisionRectH)
{ 
	setPosition(_position.x() + collisionOffsetX, _position.y() + collisionOffsetY);
	EventDispatcher::AddListener(this, EventType::PLAYER_ENEMY_COLLIDED);
}

GameObject::~GameObject() {
	EventDispatcher::RemoveListener(this, EventType::PLAYER_ENEMY_COLLIDED);
}

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
	}
}

void GameObject::Draw(Renderer* renderer, float deltaTime) {
    Rect dst = Rect(_position.x() + (_velocity.x() * deltaTime), _position.y() + (_velocity.y() * deltaTime), 
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

bool GameObject::OnNotify(Event* event){
	if(event->type() == EventType::PLAYER_ENEMY_COLLIDED) {
		_velocity.setX(0.f);
		_velocity += Vector2D(10.f, -10.f);
	}

	return false;
}  