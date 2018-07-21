#include "../include/EvilSonic.h"

#include "../include/PhysicsEngine.h"
#include "../include/Rect.h"
#include "../include/Renderer.h"
#include "../include/ResourceManager.h"

void EvilSonic::Update(){
	PhysicsEngine::ApplyGravity(this);
	PhysicsEngine::MoveAndCheckCollision(this);

	if(!_unresolvedCollisionEvents.empty()){
		while(!_unresolvedCollisionEvents.empty()){
			std::cout << "Enemy collided! " ;
			_unresolvedCollisionEvents.pop();
		}
	}
}

void EvilSonic::Draw(Renderer* renderer, float positionInterpolation) {
	Rect src = Rect(0, 0, _w, _h);
	Rect dest = Rect(_position.x(), _position.y(), _w, _h);

	renderer->Draw(ResourceManager::GetTexture("sonic"), &src, &dest);
}
