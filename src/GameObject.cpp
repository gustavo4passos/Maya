#include "../include/GameObject.h"

#include "../include/Event.h"
#include "../include/EventDispatcher.h"
#include "../include/InputModule.h"
#include "../include/PhysicsEngine.h"
#include "../include/Renderer.h"
#include "../include/ResourceManager.h"
#include "../include/SoundPlayer.h"

GameObject::GameObject(float x, float y, int w, int h) 
:	GameObject(CollisionRect(x, y, w, h, 0, 0), w, h)
{ }

GameObject::GameObject(const CollisionRect& collisionRect, int spriteW, int spriteH)
:	_velocity(0.f, 0.f),
	_spriteW(spriteW),
	_spriteH(spriteH),
	_collisionRect(collisionRect),
	_speed(2.5f),
	_impulse(8.f),
	_movingleft(false),
	_movingright(false)
{
	EventDispatcher::AddListener(this, EventType::PLAYER_ENEMY_COLLIDED); 
}

GameObject::~GameObject() {
	EventDispatcher::RemoveListener(this, EventType::PLAYER_ENEMY_COLLIDED);
}

void GameObject::setPosition(float x, float y) {
		_collisionRect.setPosition(x, y);
}

void GameObject::setVelocity(float x, float y) {
		_velocity.setX(x);
		_velocity.setY(y);
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
    Rect dst = Rect(_collisionRect.originX() + (_velocity.x() * deltaTime), _collisionRect.originY() + (_velocity.y() * deltaTime), 
     _spriteW,  _spriteH);

	Rect src = Rect(0, 0, _spriteW, _spriteH);

	if(_velocity.x() != 0.f || _velocity.y() != 0.f){
		int frame = (SDL_GetTicks() / 80) % 8;
		src.setX(src.x() + frame * _spriteW);

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
		_velocity += Vector2D(10.f, -3.f);
	}

	return false;
}  
