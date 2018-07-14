#ifndef PLAYSTATE_H
#define PLAYSTATE_H

#include <string>
#include "GameState.h"
#include "Renderer.h"

class GameEnemy;
class InfoMenuGL3;

class PlayState : public GameState {
public:    
    PlayState(){}
    ~PlayState(){}

    virtual void HandleInput();
    virtual void Update();
    virtual void Render(Renderer* renderer, float deltatime);
    virtual bool OnEnter();
    virtual bool OnExit();

    virtual const std::string& stateID(){
        return _playID;
    }

private:
    Camera* _camera;
	GameObject* _object;
	InfoMenuGL3* _infoMenu;
	Level* _level;

    static const std::string _playID;

};

#endif