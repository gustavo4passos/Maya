#ifndef GAMESTATEMACHINE_H
#define GAMESTATEMACHINE_H

#include <stack>
#include "../include/GameState.h"

class GameStateMachine{
public:

    GameStateMachine(){}
	~GameStateMachine();
	void PushState(GameState* _state); 
	void ChangeState(GameState* _state); 
	void Update(); 
	void Render();
	void PopState();

private:

    std::stack<GameState*> _gameStates;
};

#endif