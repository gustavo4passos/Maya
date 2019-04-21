#ifndef GAMESTATEMACHINE_H
#define GAMESTATEMACHINE_H

#include <stack>

#include "GameState.h"
#include "EventListener.h"

class InfoMenuGL3;

class GameStateMachine {
public:
	static void PushState(GameState* _state); 
	static void PopState();
	static void ChangeState(GameState* _state); 

	static void HandleInput();
	static void Update(); 
	static void Render(Renderer* renderer, float deltatime);

	static void Clean();

	friend class InfoMenuGL3;

private:
	GameStateMachine(){}
    static std::stack<GameState*> _gameStates;
};

#endif