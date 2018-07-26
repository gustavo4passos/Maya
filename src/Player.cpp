#include "../include/Player.h"
#include "../include/InputModule.h"
#include "../include/ResourceManager.h"
#include "../include/Renderer.h"

Player::Player(float x, float y, int w, int h): GameObject(x, y, w, h)
{}

Player::Player(const CollisionRect& collisionRect, int spriteW, int spriteH) : GameObject(collisionRect, spriteW, spriteH)
{}

Player::~Player()
{}

void Player::Draw(Renderer* renderer, float deltaTime)
{
	float drawX = _collisionRect.originX();
	//if(_currentState == STAND_ATTACK) drawX -= 8;
	Vector2D refacPosition = Vector2D(drawX + (_velocity.x() * deltaTime), _collisionRect.originY() + (_velocity.y() * deltaTime));
	Rect dst = Rect(refacPosition, _spriteW,  _spriteH);
	Rect src = Rect(_currentFrame*_spriteW, _currentRow*_spriteH, _spriteW, _spriteH);

	// Color blue = {0, 0, 200, 50};

	// renderer->DrawFillRect(&dst, &blue);

	if(_facingright){
		renderer->Draw(ResourceManager::GetTexture(_textureName), &src, &dst);
	}
	else{
		renderer->Draw(ResourceManager::GetTexture(_textureName), &src, &dst, true);
	}	
}

void Player::HandleInput()
{
    GameObject::HandleInput();
}

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

bool Player::OnNotify(Event* event){
	return GameObject::OnNotify(event);
}
