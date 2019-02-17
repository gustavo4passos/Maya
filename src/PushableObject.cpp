#include "../include/PushableObject.h"

#include "../include/Renderer.h"
#include "../include/ResourceManager.h"

PushableObject::PushableObject(float x, float y): PushableObject(CollisionRect(x, y, 47, 35, 0, 2), 47, 41)
{}

PushableObject::PushableObject(const CollisionRect& collisionRect, int spriteW, int spriteH, std::string texture) : GameObject(collisionRect, spriteW, spriteH)
{
    _collisionRect.setCollisionBehavior(CollisionBehavior::BLOCK);
    _textureName = texture;
    _kind = Kind::ZONE;
    _numFrames = 1;
    _numRows = 1;
    _currentFrame = 0;
    _currentRow = 0;
}

PushableObject::~PushableObject()
{}

void PushableObject::Update(){

    GameObject::Update();

    PhysicsEngine::ApplyGravity(this);
	PhysicsEngine::MoveAndCheckCollision(this);    

    if(PhysicsEngine::OnGround(this)){
		_velocity.setY(0.f);
	}

	_velocity.setX(0.f);

    while(!_unresolvedCollisionEvents.empty()){
        CollisionEvent e = _unresolvedCollisionEvents.front();

        if(e.kind == Kind::PLAYER){
            if(e.collisionPosition == CollisionPosition::LEFT_COLLISION ||
                e.collisionPosition == CollisionPosition::RIGHT_COLLISION){
                _velocity.setX(e.velocity.x() / 2);
            }
        }

        _unresolvedCollisionEvents.pop();
    }

}

void PushableObject::Draw(Renderer* renderer, float deltaTime)
{
	Vector2D refacPosition = Vector2D(_collisionRect.originX() + (_velocity.x() * deltaTime), _collisionRect.originY() + (_velocity.y() * deltaTime));
	Rect dst = Rect(refacPosition, _spriteW,  _spriteH);
	Rect src = Rect(_currentFrame*_spriteW, _currentRow*_spriteH, _spriteW, _spriteH);
	if(_facingright){
		renderer->Draw(ResourceManager::GetTexture(_textureName), &src, &dst);
	}
	else{
		renderer->Draw(ResourceManager::GetTexture(_textureName), &src, &dst, true);
	}	
}