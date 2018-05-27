#include "../include/Player.h"
#include "../include/InputModule.h"

Player::Player() : GameEntity()
{}

Player::~Player()
{
    if(_texture != NULL) delete _texture;
}

void Player::Load(int xPos, int yPos, int width, int height, std::string textureFile, float scale, bool flip)
{
    GameEntity::Load(xPos, yPos, width, height, textureFile, scale, flip);
}

 bool Player::Draw(Renderer* renderer)
 {
    return GameEntity::Draw(renderer);
 }

bool Player::Update(unsigned int frameTime)
{
    if(_loaded)
    {
        HandleInput();
        GameEntity::Update(frameTime);
    }
    return false;
}

void Player::HandleInput()
{
    Vector2D vec = InputModule::mousePosition();
    _position.setX(vec.x()-(/*_scale**/(_width/2)));
    _position.setY(vec.y()-(/*_scale**/(_height/2)));
}

void Player::Clean()
{
    GameEntity::Clean();
}