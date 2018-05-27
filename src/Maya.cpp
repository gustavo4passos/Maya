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
        Vector2D prevVelocity = _velocity; 
        HandleInput();
        Player::Update(frameTime);

        // Max velocity
        if(_velocity.x() >= 500) _velocity.setX(500);
        if(_velocity.x() <= -500)_velocity.setX(-500);

        // Break suavization
        // If was going right and acceleration points to left, don't let it go under zero.
        if(prevVelocity.x() > 0 && _acceleration.x() < 0 && _velocity.x() < 0)
        {
            _velocity.setX(0);
            _acceleration.setX(0);
        }
        // If was going left and acceleration points to right, don't let it go over zero.
        if(prevVelocity.x() < 0 && _acceleration.x() > 0 && _velocity.x() > 0)
        {
            _velocity.setX(0);
            _acceleration.setX(0);
        }
    }
    return false;
}

void Maya::HandleInput()
{
    Vector2D velocityNow = _velocity;

    // Walking
    if(InputModule::IsKeyPressed(InputModule::RIGHT) && InputModule::IsKeyPressed(InputModule::LEFT)){
       if(velocityNow.x() < 0) _acceleration.setX(0.4);
       else if(velocityNow.x() > 0) _acceleration.setX(-0.4);
       else _acceleration.setX(0);
    }    
    else if (InputModule::IsKeyPressed(InputModule::RIGHT)) _acceleration.setX(0.3);
    else if (InputModule::IsKeyPressed(InputModule::LEFT)) _acceleration.setX(-0.3);

    // If stopped to go any some direction
    if(InputModule::WasKeyReleased(InputModule::LEFT) && velocityNow.x() < 0) _acceleration.setX(0.3);
    if(InputModule::WasKeyReleased(InputModule::RIGHT) && velocityNow.x() > 0) _acceleration.setX(-0.3);
}
void Maya::Clean()
{
    Player::Clean();
}