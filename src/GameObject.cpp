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
	_movingleft(false),
	_movingright(false),
	_facingright(true)
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
	_frameTime++;
    _currentFrame = (_frameTime/5) % _numFrames;
    _currentRow = (_frameTime/(5*_numFrames)) % _numRows;
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

void GameObject::Draw(Renderer* renderer, float deltaTime) {}

bool GameObject::OnNotify(Event* event){
	if(event->type() == EventType::PLAYER_ENEMY_COLLIDED) {
		_velocity.setX(0.f);
		_velocity += Vector2D(10.f, -3.f);
	}

	return false;
}  
