#include "../include/GameStateMachine.h"
#include "../include/ErrorHandler.h"

std::stack<GameState*> GameStateMachine::_gameStates;

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

void GameStateMachine::HandleInput(){
    if(!_gameStates.empty()){
        _gameStates.top()->HandleInput();
    }
}

void GameStateMachine::Update(){
    if(!_gameStates.empty()){
        _gameStates.top()->Update();
    }
}

void GameStateMachine::Render(Renderer* renderer, float deltatime){
    if(!_gameStates.empty()){
        _gameStates.top()->Render(renderer, deltatime);
    }
}
