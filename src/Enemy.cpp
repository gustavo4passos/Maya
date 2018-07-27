#include "../include/Enemy.h"
#include "../include/InputModule.h"
#include "../include/ResourceManager.h"
#include "../include/Renderer.h"
#include "../include/EventDispatcher.h"

Enemy::Enemy(float x, float y, int w, int h): GameObject(x, y, w, h)
{
	EventDispatcher::AddListener(this, EventType::PLAYER_ENEMY_COLLIDED);
}

Enemy::Enemy(const CollisionRect& collisionRect, int spriteW, int spriteH) : GameObject(collisionRect, spriteW, spriteH)
{
	EventDispatcher::AddListener(this, EventType::PLAYER_ENEMY_COLLIDED);
}

Enemy::~Enemy()
{}

void Enemy::Draw(Renderer* renderer, float deltaTime)
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

void Enemy::Update()
{
    GameObject::Update();

    PhysicsEngine::ApplyGravity(this);
    PhysicsEngine::MoveAndCheckCollision(this);

    if(PhysicsEngine::OnGround(this)){
		_velocity.setY(0.f);
	}

    if(PhysicsEngine::OnWall(this)){
		_velocity.setX(0.f);
	}
}

bool Enemy::OnNotify(Event* event){
	
	// if(event->type() == EventType::PLAYER_ENEMY_COLLIDED) {
	// 	_velocity.setY(-10);
	// }

	return false;
}