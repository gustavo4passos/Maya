#include "../include/Player.h"
#include "../include/InputModule.h"
#include "../include/ResourceManager.h"
#include "../include/Renderer.h"
#include "../include/EventDispatcher.h"

Player::Player(float x, float y, int w, int h): GameObject(x, y, w, h)
{
	EventDispatcher::AddListener(this, EventType::PLAYER_ENEMY_COLLIDED);
}

Player::Player(const CollisionRect& collisionRect, int spriteW, int spriteH) : GameObject(collisionRect, spriteW, spriteH)
{
	EventDispatcher::AddListener(this, EventType::PLAYER_ENEMY_COLLIDED);
}

Player::~Player()
{}

void Player::Draw(Renderer* renderer, float deltaTime)
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

void Player::HandleInput() { }

void Player::Update()
{
    GameObject::Update();

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
		_velocity.setX(0.f);
	}
}

bool Player::OnNotify(Event* event){}
