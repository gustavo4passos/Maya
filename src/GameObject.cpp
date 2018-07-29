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
:	_collisionRect(collisionRect),
	_spriteW(spriteW),
	_spriteH(spriteH),	
	_facingright(true),
	_velocity(0.f, 0.f),
    _frameTime(0)
{}

GameObject::~GameObject() 
{ }

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

void GameObject::Draw(Renderer* renderer, float deltaTime) {}

bool GameObject::OnNotify(Event* event)
{}
