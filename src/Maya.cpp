#include "../include/Maya.h"

#include <iostream>

#include "../include/InputModule.h"
#include "../include/ResourceManager.h"
#include "../include/Weapon.h"

Maya::Maya(float x, float y): Maya(CollisionRect(x, y, 10, 30, 12, 7), 36, 39)
{}

Maya::Maya(const CollisionRect& collisionRect, int spriteW, int spriteH) : Player(collisionRect, spriteW, spriteH)
{
    _textureName = "maya_standing";
    _currentState = STAND;
    _kind = Kind::PLAYER;
    _speed = 2.5f;
	_impulse = 8.f;
    _weapon = new Weapon(x(),y(), 23, 4);
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
             _weapon->collisionRectCHANGEBLE().setCollisionBehavior(CollisionBehavior::IGNORE); 
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
            _collisionRect.setOffsetX(12);
            _currentState = JUMP;
            _numFrames = 3;
            _numRows = 3;
            _spriteW = 38;
            _spriteH = 41;
            _textureName = "maya_jumping";            
        }

        else if (state == STAND_ATTACK){
            _collisionRect.setOffsetX(_collisionRect.offsetX()+8);
            _currentState = STAND_ATTACK;
            _numFrames = 3;
            _numRows = 2;
            _spriteW = 53;
            _spriteH = 38;
            _textureName = "maya_attacking";            
            SoundPlayer::PlaySFX(ResourceManager::GetSoundEffect("dagger_swing"));
        }

        else if (state == BOUNCE_STUCK){
            _collisionRect.setOffsetX(12);
            _currentState = BOUNCE_STUCK;
            _numFrames = 3;
            _numRows = 3;
            _spriteW = 38;
            _spriteH = 41;
            _textureName = "maya_jumping";           
        }
    }
}

void Maya::Update()
{  
    Player::Update();

    _weapon->setPosition(x(),y());
   
    
    if (_currentState == JUMP){
        if (PhysicsEngine::OnGround(this)){
            ChangeState(STAND);
        }
    }
    
    else if (_currentState == BOUNCE_STUCK){
        if(PhysicsEngine::OnGround(this)){
            ChangeState(STAND);
        }
    }

    else if (_currentState == STAND_ATTACK){
        _velocity.setX(0);        
        if((_currentFrame >= 1 && _currentFrame <= 2 && _currentRow == 0)
        || (_currentFrame == 0 && _currentRow == 1))
        {
            if(_facingright) _weapon->setPosition(x()+11, y()+10);            
            else _weapon->setPosition(x()-20, y()+10);
            
            _weapon->_collisionRect.setCollisionBehavior(CollisionBehavior::BLOCK);
            
            if(PhysicsEngine::OnWall(_weapon) && _currentFrame == 1){
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
    
    while(!_unresolvedCollisionEvents.empty()){
        CollisionEvent e = _unresolvedCollisionEvents.front();

        if(e.kind == Kind::ENEMY){
            if(_facingright) _velocity = Vector2D(-2, -4);
            else             _velocity = Vector2D(2, -4);
            ChangeState(BOUNCE_STUCK);
        }

        _unresolvedCollisionEvents.pop();        
    }
      
}


bool Maya::OnNotify(Event* event){
    // if(event->type() == EventType::PLAYER_ENEMY_COLLIDED){
    //     //_velocity.setX(0);
    //     
    // }
    return false;
}
