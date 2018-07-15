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
}

void EvilSonic::Draw(Renderer* renderer, float deltaTime) {
	Rect src = Rect(0, 0, _w, _h);
	Rect dest = Rect(_position.x() + _velocity.x() * deltaTime, _position.y() + _velocity.y() * deltaTime
	, _w, _h);

	renderer->Draw(ResourceManager::GetTexture("maya_standing"), &src, &dest);
}

bool EvilSonic::OnNotify(Event* event) {
	if(event->type() == EventType::PLAYER_ENEMY_COLLIDED) {
		_velocity.setX(_velocity.x() + 0.5);
		return false;
	}

	return false;
}