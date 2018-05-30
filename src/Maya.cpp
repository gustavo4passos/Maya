#include "../include/Maya.h"

#include <iostream>

#include "../include/InputModule.h"


Maya::Maya() : Player()
{}

Maya::~Maya()
{}

bool Maya::Load(int xPos, int yPos, int width, int height, TextureID sprite, float scale, bool flip)
{
    Player::Load(xPos, yPos, width, height, sprite, scale, flip);
    _numFrames = 8;
}

void Maya::Draw(Renderer* renderer, float positionFactor)
{
    Player::Draw(renderer, positionFactor);
}

void Maya::HandleInput()
{
    _velocity.setX(0);

    if(InputModule::IsKeyPressed(InputModule::RIGHT)) {
        _velocity.setX(_velocity.x()+5);
    }

    if(InputModule::IsKeyPressed(InputModule::LEFT)) {
        _velocity.setX(_velocity.x()-5);
    }

}

void Maya::Update()
{  
     Player::Update();
}

void Maya::Clean()
{
    Player::Clean();
}