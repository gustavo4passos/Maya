#include "../include/Golem.h"

#include "../include/ServiceLocator.h"
#include "../include/GameSwitches.h"

Golem::Golem(float x, float y, const std::string& switchRequired)
:	Enemy(CollisionRect(Rect(x,y,26,23),CollisionBehavior::BLOCK,14,33),52,56),
	_switchRequired(switchRequired)
{ 
	ChangeState(CROUCH);
}

void Golem::Draw(Renderer* renderer, float deltaTime){
	Enemy::Draw(renderer, deltaTime);
}

bool Golem::OnNotify(Event* event){
	if(event->type() == EventType::PLAYER_ENEMY_COLLIDED) {}
	return false;
}

void Golem::Update(){
	Enemy::Update();

	if(_currentState == CROUCH){
		if(ServiceLocator::GetGameSwitches()->CheckSwitch(_switchRequired) && _currentState == CROUCH){
        ChangeState(WALK);
	}
	
    else if(_currentState == WALK){
    	if(x()-_startPosition.x() == 130){
    		_velocity.setX(-0.5);
    		_facingright = false;
    	}
    	else if(x() == _startPosition.x()){
    		_velocity.setX(0.5);
    		_facingright = true;
    	}
    }


}

void Golem::ChangeState(GolemState state){
 	_frameTime = 0;

 	if(state == CROUCH){
 		_currentState = CROUCH;
 		_numRows = 1;
 		_numFrames = 1;
 		_textureName = "../res/assets/static-golem.png";
 	}
 	else if(state == WALK){
 		_currentState = WALK;
        _textureName = "../res/assets/golem-walk.png";
        _numRows = 3;
        _numFrames = 4;
        _collisionRect = CollisionRect(Rect((x()-12),(y()-33),40,53)
        	,CollisionBehavior::BLOCK,6,3);
        _velocity.setX(0.5);
        _facingright = true;
        
        _startPosition.setX(x()-12);
        _startPosition.setY(y()-33);
 	}

}
