#include "../include/Golem.h"

#include <iostream>
#include "../include/ServiceLocator.h"
#include "../include/GameSwitches.h"
#include "../include/PhysicsEngine.h"

Golem::Golem(float x, float y, const std::string& switchRequired)
:	Enemy(CollisionRect(Rect(x,y,26,23),CollisionBehavior::BLOCK,14,33),52,56),
    _switchRequired(switchRequired)
{
	if (switchRequired == "")
    {
        _collisionRect = CollisionRect(Rect((x-12),(y-33),28,50) , CollisionBehavior::BLOCK,14,6);
        ChangeState(WALK);
    }
    else{
        ChangeState(CROUCH);
    }
	
    _kind = Kind::ENEMY;
}

void Golem::Draw(Renderer* renderer, float deltaTime){
	Enemy::Draw(renderer, deltaTime);
}

bool Golem::OnNotify(Event* event){
	return false;
}

void Golem::Update(){

	Enemy::Update();

	if(_currentState == CROUCH){
		if(_switchRequired != "" && ServiceLocator::GetGameSwitches()->CheckSwitch(_switchRequired))
        {
            _collisionRect = CollisionRect(Rect((x()-12),(y()-33),28,50) , CollisionBehavior::BLOCK,14,6);
            ChangeState(WALK);
        }
	}
	else if(_currentState == WALK){
    	if( ((ServiceLocator::GetPlayer())->x() - x()) < 100 && _facingright && ((ServiceLocator::GetPlayer())->x() - x()) > 0)
            ChangeState(CHASING);
        else if( ((ServiceLocator::GetPlayer())->x() - x()) > -100 && !_facingright && ((ServiceLocator::GetPlayer())->x() - x()) < 0)
            ChangeState(CHASING);
        else StandWalk();
    }
    else if(_currentState == CHASING){
        if( ((ServiceLocator::GetPlayer())->x() - x()) > 145 || ((ServiceLocator::GetPlayer())->x() - x()) < -145)
            ChangeState(WALK);

        else if( ((ServiceLocator::GetPlayer())->x() - x()) <= 60 && ((ServiceLocator::GetPlayer())->x() - x()) >= -60 && _frameTime >= 60)
            ChangeState(ATTACKING);

        else if( ((ServiceLocator::GetPlayer())->x() - x()) >= 0){
            _velocity.setX(1.f);
            _facingright = true;
        }
        else {
            _velocity.setX(-1.f);
            _facingright = false;
        }
    }
    else if(_currentState == ATTACKING){
        if(_currentFrame == 3 && _currentRow == 2){
            ChangeState(CHASING);
            _attacking = false;
        }
        else if( (_currentFrame == 3 && _currentRow == 2) || !_attacking){
            if(((ServiceLocator::GetPlayer())->x() - x()) <= 0){
                _velocity.setX(-1.5);
                _facingright = false;
                _attacking = true;
            }   
            else if(((ServiceLocator::GetPlayer())->x() - x()) > 0){
                _velocity.setX(1.5);
                _facingright = true;
                _attacking = true;
            }
        }
    }

    else if (_currentState == STUCK){
        _velocity.setX(0);
        if(_frameTime >= 60) ChangeState(WALK);
    }
    
    if(_currentState != CROUCH){
        while(!_unresolvedCollisionEvents.empty()){
            CollisionEvent event = _unresolvedCollisionEvents.front();
            _unresolvedCollisionEvents.pop();

            if(event.kind == Kind::WEAPON) {
                _life -= event.damage;
                _velocity.setX(0);
                if(_life == 0) ChangeState(DEAD);
            }        
        }
    }
}

void Golem::StandWalk(){
    if(x()-_startPosition.x() >= 130 || PhysicsEngine::OnWallRight(this)){
        _velocity.setX(-0.5);
        _facingright = false;
    }
    else if(x() <= _startPosition.x() || x()-_startPosition.x() == 0 || PhysicsEngine::OnWallLeft(this)){
        _velocity.setX(0.5);
        _facingright = true;
    }
}

void Golem::ChangeState(GolemState state){
 	_frameTime = 0;

 	if(state == CROUCH){
 		_currentState = CROUCH;
 		_numRows = 1;
 		_numFrames = 1;
 		_textureName = "static-golem";
 	}
    else if(state == WALK){
        _currentState = WALK;
        _startPosition.setX(x());
        _startPosition.setY(y());
        _textureName = "golem-walk";
        _numRows = 4;
        _numFrames = 4;
    }
    else if(state == CHASING){
        _currentState = CHASING;
        _textureName = "golem-walk";
        _numRows = 4;
        _numFrames = 4;
    }
    else if(state == ATTACKING){
        _currentState = ATTACKING;
        _textureName = "golem-attack";
        _numRows = 3;
        _numFrames = 4;
    }
    else if(state == DEAD){
        _currentState = DEAD;
        _numRows = 1;
        _numFrames = 1;
        _textureName = "static-golem";
        _collisionRect = CollisionRect(Rect(x()+12,y()+28,26,23),CollisionBehavior::BLOCK,14,33);
    }
    else if (state == STUCK){
        _currentState = STUCK;
        _textureName = "golem-walk";
        _numRows = 1;
        _numFrames = 1;
    }
}