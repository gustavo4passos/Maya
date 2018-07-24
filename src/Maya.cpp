#include "../include/Maya.h"

#include <iostream>

#include "../include/InputModule.h"
#include "../include/ResourceManager.h"


Maya::Maya(float x, float y, int w, int h): Player(x, y, w, h)
{
    Maya(CollisionRect(x, y, w, h, 0, 0), w, h);
}

Maya::Maya(const CollisionRect& collisionRect, int spriteW, int spriteH) : Player(collisionRect, spriteW, spriteH)
{
     _textureName = "maya_standing";
     _currentState = STAND;
}

Maya::~Maya()
{}

// void Maya::Load(int xPos, int yPos, int width, int height, std::string sprite, float scale, bool flip)
// {
//     Player::Load(xPos, yPos, width, height, sprite, scale, flip);
//     ChangeState(STAND);
// }

void Maya::Draw(Renderer* renderer, float positionFactor)
{
    Player::Draw(renderer, positionFactor);
}

void Maya::HandleInput()
{

    bool leftPressed = InputModule::IsKeyPressed(InputModule::LEFT);
    bool rightPressed = InputModule::IsKeyPressed(InputModule::RIGHT);

    if(leftPressed) _facingright = false;
    if(rightPressed) _facingright = true;

    if  (_currentState == STAND){
        _velocity.setX(0);
        if (InputModule::WasKeyPressed(InputModule::SPACE)){
            if(PhysicsEngine::OnGround(this)){
                _velocity.setY(-_impulse);
            }
        } 
        else if (InputModule::WasKeyPressed(InputModule::LCTRL)) ChangeState(STAND_ATTACK);
        else if ((leftPressed && !rightPressed) || (rightPressed && !leftPressed)) ChangeState(RUN);
    } 

    else if (_currentState == JUMP){
        _velocity.setX(0);
        if (leftPressed)  _velocity.setX(-2); 
        else if (rightPressed) _velocity.setX(2);
    }

    else if (_currentState == RUN){
       
        if (InputModule::IsKeyPressed(InputModule::SPACE)){
            if(PhysicsEngine::OnGround(this)){
                _velocity.setY(-_impulse);
            }
        }        
        else if ((leftPressed && rightPressed) || (!leftPressed && !rightPressed)){
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
        _spriteW = 36;
        _spriteH = 39;

        if (state == STAND){
            _currentState = STAND;        
            _numFrames = 1;
            _numRows = 1;
            _textureName = "maya_standing";
        }

        else if (state == RUN){
            _currentState = RUN;            
            _numFrames = 4;
            _numRows = 2;
            _textureName = "maya_running";          
        }

        else if (state == JUMP){
            _currentState = JUMP;
            _numFrames = 3;
            _numRows = 3;
            _spriteW = 38;
            _spriteH = 41;
            _textureName = "maya_jumping";            
        }

        else if (state == STAND_ATTACK){
            _currentState = STAND_ATTACK;
            _numFrames = 3;
            _numRows = 2;
            _spriteW = 46;
            _spriteH = 38;
            _textureName = "maya_attacking";            
            SoundPlayer::PlaySFX(ResourceManager::GetSoundEffect("dagger_swing"));
        }

    }
}

void Maya::Update()
{  
    Player::Update();
    
    if (_currentState == JUMP){
        if (PhysicsEngine::OnGround(this)){
            ChangeState(STAND);
        }
    }

    else if (_currentState == STAND_ATTACK){
        _velocity.setX(0);
        if (_currentFrame == 2 && _currentRow == 1) ChangeState(STAND);
    }

    else if (_currentState == STAND){
        if(!PhysicsEngine::OnGround(this)){
            ChangeState(JUMP);
        }
    }

    else if (_currentState == RUN){
        if(!PhysicsEngine::OnGround(this)){
            ChangeState(JUMP);
        }
    }

    
}


bool Maya::OnNotify(Event* event){
    Player::OnNotify(event);
}

// void Maya::Clean()
// {
//     Player::Clean();
// }

