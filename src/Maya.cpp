#include "../include/Maya.h"

#include <iostream>

#include "../include/InputModule.h"
#include "../include/ResourceManager.h"


Maya::Maya() : Player()
{}

Maya::~Maya()
{}

void Maya::Load(int xPos, int yPos, int width, int height, std::string sprite, float scale, bool flip)
{
    Player::Load(xPos, yPos, width, height, sprite, scale, flip);
    ChangeState(STAND);
}

void Maya::Draw(Renderer* renderer, float positionFactor)
{
    Player::Draw(renderer, positionFactor);
}

void Maya::HandleInput()
{

    bool leftPressed = InputModule::IsKeyPressed(InputModule::LEFT);
    bool rightPressed = InputModule::IsKeyPressed(InputModule::RIGHT);

    if  (_currentState == STAND){
        _velocity.setX(0);
        if      (InputModule::IsKeyPressed(InputModule::SPACE)) ChangeState(JUMP);
        else if (InputModule::WasKeyPressed(InputModule::LCTRL)) ChangeState(STAND_ATTACK);
        else if ((leftPressed && !rightPressed) || (rightPressed && !leftPressed)) ChangeState(RUN);
    } 

    else if (_currentState == JUMP){
        _velocity.setX(0);
        if (leftPressed)  _velocity.setX(-2); 
        else if (rightPressed) _velocity.setX(2);
    }

    else if (_currentState == RUN){
        if ((leftPressed && rightPressed) || (!leftPressed && !rightPressed)){
            ChangeState(STAND);
        }            
        else if (InputModule::WasKeyPressed(InputModule::LCTRL)){
            ChangeState(STAND_ATTACK);
        }           
        else if (leftPressed)  _velocity.setX(-2); 
        else if (rightPressed) _velocity.setX(2);       
    }    
}

void Maya::ChangeState(PlayerState state)
{    
    if(_currentState != state){
        
        _frameTime = 0;
        _width = 36;
        _height = 39;

        if (state == STAND){
            _currentState = STAND;        
            _numFrames = 6;
            _numRows = 3;
            _defaultTexture = "maya_stand";
        }

        else if (state == RUN){
            _currentState = RUN;            
            _numFrames = 4;
            _numRows = 2;
            _defaultTexture = "maya_run";          
        }

        else if (state == JUMP){
            _currentState = JUMP;
            _numFrames = 3;
            _numRows = 3;
            _width = 38;
            _height = 41;
            _defaultTexture = "maya_jump";            
        }

        else if (state == STAND_ATTACK){
            _currentState = STAND_ATTACK;
            _numFrames = 3;
            _numRows = 2;
            _width = 46;
            _height = 38;
            _defaultTexture = "maya_attack";            
        }

    }
}

void Maya::Update()
{  
    Player::Update();
    
    if (_currentState == JUMP){
        if (_currentFrame == 2 && _currentRow == 2) ChangeState(STAND);
    }

    else if (_currentState == STAND_ATTACK){
        _velocity.setX(0);
        if (_currentFrame == 2 && _currentRow == 1) ChangeState(STAND);
    }
    
}

void Maya::Clean()
{
    Player::Clean();
}

