#include "../include/GameStateMachine.h"
#include "../include/ErrorHandler.h"

void GameStateMachine::PushState(GameState* _state){
    _gameStates.push(_state);
    _gameStates.top()->OnEnter();
}

void GameStateMachine::PopState(){
    if(!_gameStates.empty()){

        if(_gameStates.top()->OnExit()){

            GameState* state = _gameStates.top();
            _gameStates.pop();
            delete state;
            return;
        } 

        LOG_ERROR("Fail to execute OnExit function");
    }

    LOG_ERROR("The stack is empty");
    
}

void GameStateMachine::ChangeState(GameState* _state){
    while(!_gameStates.empty()){

        if(_gameStates.top()->stateID() == _state->stateID()){
            LOG_ERROR("Do nothing, already in this state");
            return;

        }

        if(_gameStates.top()->OnExit()){

            GameState* state = _gameStates.top();
            _gameStates.pop();
            delete state;

        }
    }
    _gameStates.push(_state);
    _state->OnEnter();
}

void GameStateMachine::Update(){
    if(!_gameStates.empty()){
        _gameStates.top()->Update();
    }
}

void GameStateMachine::Render(){
    if(!_gameStates.empty()){
        _gameStates.top()->Update();
    }
}
