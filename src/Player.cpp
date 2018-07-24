#include "../include/Player.h"
#include "../include/InputModule.h"

Player::Player(float x, float y, int w, int h): GameObject(x, y, w, h)
{}

Player::Player(const CollisionRect& collisionRect, int spriteW, int spriteH) : GameObject(collisionRect, spriteW, spriteH)
{}

Player::~Player()
{}

// void Player::Load(int xPos, int yPos, int width, int height, std::string sprite, float scale, bool flip)
// {
//     GameEntity::Load(xPos, yPos, width, height, sprite, scale, flip);
// }

void Player::Draw(Renderer* renderer, float positionFactor)
{
   GameObject::Draw(renderer, positionFactor);
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
		// if(_movingright) _facingright = true;
		// if(_movingleft) _facingright = false;
		_velocity.setX(0.f);
	}
}

bool Player::OnNotify(Event* event){
	 GameObject::OnNotify(event);
}

// void Player::Clean()
// {
//     GameEntity::Clean();
// }