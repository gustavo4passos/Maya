#ifndef PLAYSTATE_H
#define PLAYSTATE_H

#include <string>
#include "GameState.h"
#include "Renderer.h"
#include "Maya.h"

class GameEnemy;
class InfoMenuGL3;
class Region;

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
	Player* _maya;
	InfoMenuGL3* _infoMenu;
    Region* _region;

    static const std::string _playID;

};

#endif