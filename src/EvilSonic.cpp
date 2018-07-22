#include "../include/EvilSonic.h"

#include "../include/Event.h"
#include "../include/PhysicsEngine.h"
#include "../include/Rect.h"
#include "../include/Renderer.h"
#include "../include/ResourceManager.h"

void EvilSonic::Update(){
	if(_velocity.x() == 0.f) {
		_velocity.setX(1.f);
	}
	if(PhysicsEngine::OnGround(this)) {
		_velocity.setY(0.f);
	}
	if(PhysicsEngine::OnWall(this)) {
		_velocity.setY(-3.f);
	}

	PhysicsEngine::ApplyGravity(this);
	PhysicsEngine::MoveAndCheckCollision(this);
	
	if(!_unresolvedCollisionEvents.empty()){
		while(!_unresolvedCollisionEvents.empty()){
			_unresolvedCollisionEvents.pop();
		}
	}

	if(y() > 800.f) {
		setPosition(0.f, 0.f);
		_velocity.setX(1.f);
	}
}

void EvilSonic::Draw(Renderer* renderer, float deltaTime) {
	Rect src = Rect(0, 0, _spriteW, _spriteH);
	Rect dest = Rect(_collisionRect.originX() + _velocity.x() * deltaTime, _collisionRect.originY() + _velocity.y() * deltaTime, _spriteW, _spriteH);

	renderer->Draw(ResourceManager::GetTexture("maya_standing"), &src, &dest);
}

bool EvilSonic::OnNotify(Event* event) {
	if(event->type() == EventType::PLAYER_ENEMY_COLLIDED) {
		_velocity.setX(_velocity.x() + 0.5);
		return false;
	}

	return false;
}