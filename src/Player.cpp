#include "../include/Player.h"
#include "../include/InputModule.h"

Player::Player() : GameEntity()
{}

Player::~Player()
{}

void Player::Load(int xPos, int yPos, int width, int height, std::string sprite, float scale, bool flip)
{
    GameEntity::Load(xPos, yPos, width, height, sprite, scale, flip);
}

void Player::Draw(Renderer* renderer, float positionFactor)
{
   GameEntity::Draw(renderer, positionFactor);
}

void Player::HandleInput()
{
    Vector2D vec = InputModule::GetMousePosition();
    _position.setX(vec.x()-(_scale*_width/2));
    _position.setY(vec.y()-(_scale*_height/2));
}

void Player::Update()
{
    GameEntity::Update();        
}



void Player::Clean()
{
    GameEntity::Clean();
}