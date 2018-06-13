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

	if(PhysicsEngine::OnWall(this)){
		_velocity.setX(0.f);
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
			_velocity.setY(-_impulse);
		}
	}
}

void GameObject::Draw(Renderer* renderer, float positionInterpolation) {

    Rect rect = Rect(0, 0, 32, 32);

    Rect src = Rect(_collisionRect.x() + (_velocity.x() * positionInterpolation), _collisionRect.y() + (_velocity.y() * positionInterpolation), 
     _collisionRect.w(),  _collisionRect.h());

    renderer->Draw(ResourceManager::GetTexture("grass"), &rect, &src);
}
