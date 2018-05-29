#include "../include/Maya.h"
#include "../include/InputModule.h"


Maya::Maya() : Player()
{}

Maya::~Maya()
{
    if(_texture != NULL) delete _texture;
}

void Maya::Load(int xPos, int yPos, int width, int height, std::string textureFile, float scale, bool flip)
{
    Player::Load(xPos, yPos, width, height, textureFile, scale, flip);
}

 bool Maya::Draw(Renderer* renderer)
 {
    return Player::Draw(renderer);
 }

bool Maya::Update(unsigned int frameTime)
{
    if(_loaded)
    {
        HandleInput();
        Player::Update(frameTime);
    }
    return false;
}

void Maya::HandleInput()
{

}
void Maya::Clean()
{
    Player::Clean();
}