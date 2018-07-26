#include "../include/Maya.h"

#include <iostream>

#include "../include/InputModule.h"
#include "../include/ResourceManager.h"

Maya::Maya(float x, float y): Maya(CollisionRect(x, y, 10, 30, 12, 7), 36, 39)
{}

Maya::Maya(const CollisionRect& collisionRect, int spriteW, int spriteH) : Player(collisionRect, spriteW, spriteH)
{
    _textureName = "maya_standing";
    _currentState = STAND;
    _kind = Kind::PLAYER;
    _speed = 2.5f;
	_impulse = 8.f;
    _weapon = new GameObject(_collisionRect.x(), _collisionRect.y()+16, 10, 4);
    _weapon->collisionRectCHANGEBLE().setCollisionBehavior(CollisionBehavior::IGNORE);
}

Maya::~Maya()
{}

void Maya::Draw(Renderer* renderer, float positionFactor)
{
    Player::Draw(renderer, positionFactor);
}

void Maya::HandleInput()
{

    bool leftPressed = InputModule::IsKeyPressed(InputModule::LEFT) || InputModule::StickXValue(InputModule::LEFT_STICK) == -1;
    bool rightPressed = InputModule::IsKeyPressed(InputModule::RIGHT) || InputModule::StickXValue(InputModule::LEFT_STICK) == 1;

    if(leftPressed) _facingright = false;
    if(rightPressed) _facingright = true;

    if  (_currentState == STAND){
        _velocity.setX(0);
        if (InputModule::WasKeyPressed(InputModule::SPACE) || InputModule::IsJoyButtonDown(InputModule::JOY_A)){
            if(PhysicsEngine::OnGround(this)){
                _velocity.setY(-_impulse);
            }
        } 
        else if (InputModule::WasKeyPressed(InputModule::LCTRL) || InputModule::IsJoyButtonDown(InputModule::JOY_X)) {            
            ChangeState(STAND_ATTACK);
        }
        else if ((leftPressed && !rightPressed) || (rightPressed && !leftPressed)) ChangeState(RUN);
    } 

    else if (_currentState == JUMP){
        _velocity.setX(0);
        if (leftPressed)  _velocity.setX(-2); 
        else if (rightPressed) _velocity.setX(2);
    }

    else if (_currentState == RUN){
       
        if (InputModule::IsKeyPressed(InputModule::SPACE) || InputModule::IsJoyButtonDown(InputModule::JOY_A)){
            if(PhysicsEngine::OnGround(this)){
                _velocity.setY(-_impulse);
            }
        }        
        else if ((leftPressed && rightPressed) || (!leftPressed && !rightPressed)){
            ChangeState(STAND);
        }            
        else if (InputModule::WasKeyPressed(InputModule::LCTRL) || InputModule::IsJoyButtonDown(InputModule::JOY_X)){
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
            _collisionRect.setOffsetX(12);
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
            _spriteW = 53;
            _spriteH = 38;
            _textureName = "maya_attacking";            
            SoundPlayer::PlaySFX(ResourceManager::GetSoundEffect("dagger_swing"));
        }

    }
}

void Maya::Update()
{  
    Player::Update();

    
    _collisionRect.setOffsetX(12);
    _weapon->setPosition(_collisionRect.x(), _collisionRect.y()+16);    
    
    if (_currentState == JUMP){
        if (PhysicsEngine::OnGround(this)){
            ChangeState(STAND);
        }
    }

    else if (_currentState == STAND_ATTACK){
        _velocity.setX(0);
        _collisionRect.setOffsetX(_collisionRect.offsetX()+8);
        if((_currentFrame >= 1 && _currentFrame <= 2 && _currentRow == 0)
        || (_currentFrame == 0 && _currentRow == 1))
        {
            if(_facingright) _weapon->setPosition(_collisionRect.x()+25, _collisionRect.y()+10);
            else _weapon->setPosition(_collisionRect.x()-20, _collisionRect.y()+10);
            _weapon->collisionRectCHANGEBLE().setCollisionBehavior(CollisionBehavior::BLOCK);
            std::cout << _collisionRect.offsetX() << "\n";
            if(PhysicsEngine::OnWall(_weapon)){
                SoundPlayer::PlaySFX(ResourceManager::GetSoundEffect("damage"));
            }
        }
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
    return Player::OnNotify(event);
}
